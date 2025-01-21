#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100
#define MAX_LINE_LENGTH 256
#define TRANSACTION_FILE "transactiondata.csv" // The file where transactions are logged

// Function to fetch and display transaction data in HTML format
void fetch_transactions() {
    FILE *file = fopen(TRANSACTION_FILE, "r");
    if (file == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Could not open transaction log file.</h3></body></html>");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];

    // Print HTML Table for transactions
    printf("Content-Type: text/html\n\n");
    printf("<html><body><h1>Transaction History</h1>");
    printf("<table border=\"1\" style=\"border-collapse: collapse; width: 100%%; text-align: left;\">");
    printf("<tr><th>Transaction ID</th><th>User Name</th><th>Service</th><th>Amount Paid</th><th>Date</th><th>Payment Method</th></tr>");

    // Read transaction log and display each line in the table
    while (fgets(line, sizeof(line), file)) {
        char user[MAX_INPUT_SIZE], service[MAX_INPUT_SIZE], payment[MAX_INPUT_SIZE];
        int id;
        double amount;
        char date[MAX_INPUT_SIZE];

        // Parse each line (assuming CSV format: id, user, service, amount, date, payment)
        sscanf(line, "%d,%[^,],%[^,],%lf,%[^,],%[^\n]", &id, user, service, &amount, date, payment);

        // Print each transaction as a table row
        printf("<tr><td>%d</td><td>%s</td><td>%s</td><td>%.2f</td><td>%s</td><td>%s</td></tr>", id, user, service, amount, date, payment);
    }

    printf("</table></body></html>");
    fclose(file);
}

// Function to add a new transaction to the transaction file
void add_transaction(int id, const char *user, const char *service, double amount, const char *date, const char *payment) {
    FILE *file = fopen(TRANSACTION_FILE, "a");
    if (file == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Could not open transaction log file for writing.</h3></body></html>");
        exit(1);
    }

    // Write the new transaction in CSV format
    fprintf(file, "%d,%s,%s,%.2f,%s,%s\n", id, user, service, amount, date, payment);
    fclose(file);
}

int main() {
    // Example: Add a new transaction
    int new_id = 101; // Example Transaction ID
    const char *user = "John Doe";
    const char *service = "Internet Access";
    double amount = 20.50;
    const char *date = "2025-01-21";
    const char *payment = "Credit Card";

    // Add the new transaction
    add_transaction(new_id, user, service, amount, date, payment);

    // Fetch and display all transactions
    fetch_transactions();

    return 0;
}
