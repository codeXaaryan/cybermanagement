#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INPUT_SIZE 100
#define MAX_LINE_LENGTH 256
#define TRANSACTION_FILE "transaction_log.csv"  // The file where transactions are logged

// Structure to hold transaction data
typedef struct {
    int id;
    char user[MAX_INPUT_SIZE];
    char service[MAX_INPUT_SIZE];
    double amount;
    char date[MAX_INPUT_SIZE];  // Format: YYYY-MM-DD
    char payment[MAX_INPUT_SIZE];
} Transaction;

// Function to parse transaction file and calculate revenue
void calculate_revenue(const char* time_period) {
    FILE *file = fopen(TRANSACTION_FILE, "r");
    if (file == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Could not open transaction log file.</h3></body></html>");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    double daily_revenue[31] = {0};  // Assume 31 days in a month
    double weekly_revenue[5] = {0};  // Assume max 5 weeks in a month
    double monthly_revenue[12] = {0};  // 12 months in a year
    
    // Get the current date
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int current_day = tm_info->tm_mday;
    int current_month = tm_info->tm_mon + 1;  // tm_mon is 0-based
    int current_year = tm_info->tm_year + 1900;

    // Process each line from the transaction log file
    while (fgets(line, sizeof(line), file)) {
        Transaction t;
        sscanf(line, "%d,%[^,],%[^,],%lf,%[^,],%[^\n]", &t.id, t.user, t.service, &t.amount, t.date, t.payment);
        
        // Parse the date of the transaction
        int trans_year, trans_month, trans_day;
        sscanf(t.date, "%d-%d-%d", &trans_year, &trans_month, &trans_day);
        
        // Check if the transaction is from the correct time period
        if (strcmp(time_period, "daily") == 0 && trans_day == current_day) {
            daily_revenue[trans_day - 1] += t.amount;
        } else if (strcmp(time_period, "weekly") == 0 && trans_month == current_month) {
            int week = (trans_day - 1) / 7;  // Assume each week starts on a Sunday
            weekly_revenue[week] += t.amount;
        } else if (strcmp(time_period, "monthly") == 0 && trans_year == current_year) {
            monthly_revenue[trans_month - 1] += t.amount;
        }
    }

    fclose(file);

    // Print the revenue data in JSON format for the frontend
    printf("Content-Type: application/json\n\n");
    printf("{\n");

    if (strcmp(time_period, "daily") == 0) {
        printf("\"dailyRevenue\": [");
        for (int i = 0; i < 31; i++) {
            printf("%.2f", daily_revenue[i]);
            if (i < 30) printf(", ");
        }
        printf("],\n");
    } else if (strcmp(time_period, "weekly") == 0) {
        printf("\"weeklyRevenue\": [");
        for (int i = 0; i < 5; i++) {
            printf("%.2f", weekly_revenue[i]);
            if (i < 4) printf(", ");
        }
        printf("],\n");
    } else if (strcmp(time_period, "monthly") == 0) {
        printf("\"monthlyRevenue\": [");
        for (int i = 0; i < 12; i++) {
            printf("%.2f", monthly_revenue[i]);
            if (i < 11) printf(", ");
        }
        printf("]\n");
    }

    printf("}\n");
}

int main() {
    // Get the time period from the query string (CGI)
    char* query_string = getenv("QUERY_STRING");
    if (query_string == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: No time period specified.</h3></body></html>");
        return 1;
    }

    char time_period[MAX_INPUT_SIZE];
    if (sscanf(query_string, "period=%s", time_period) != 1) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Invalid query parameter.</h3></body></html>");
        return 1;
    }

    calculate_revenue(time_period);  // Calculate and return the revenue data

    return 0;
}
