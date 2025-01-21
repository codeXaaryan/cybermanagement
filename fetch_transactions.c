#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define TRANSACTION_FILE "transactiondata.csv"  // Path to your transaction log file

void fetch_transactions() {
    FILE *file = fopen(TRANSACTION_FILE, "r");
    if (file == NULL) {
        // Respond with an error message if the file cannot be opened
        printf("Content-Type: text/plain\n\n");
        printf("<tr><td colspan=\"6\">Error: Could not open transaction log file.</td></tr>");
        return;
    }

    char line[MAX_LINE_LENGTH];

    // Output Content-Type header for the CGI script
    printf("Content-Type: text/plain\n\n");

    // Read the transaction log and format it as HTML table rows
    while (fgets(line, sizeof(line), file)) {
        char user[MAX_LINE_LENGTH], service[MAX_LINE_LENGTH], payment[MAX_LINE_LENGTH];
        int id;
        double amount;
        char date[MAX_LINE_LENGTH];

        // Parse each line (CSV format: id, user, service, amount, date, payment)
        sscanf(line, "%d,%[^,],%[^,],%lf,%[^,],%[^\n]", &id, user, service, &amount, date, payment);

        // Print each transaction as a table row
        printf("<tr><td>%d</td><td>%s</td><td>%s</td><td>%.2f</td><td>%s</td><td>%s</td></tr>\n",
               id, user, service, amount, date, payment);
    }

    fclose(file);
}

int main() {
    fetch_transactions();  // Fetch and display transaction data
    return 0;
}
