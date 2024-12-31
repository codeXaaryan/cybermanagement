#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RATE_PER_SECOND 0.1  // Example rate (adjust as needed)
#define LOG_FILE "bills.log"

void log_bill(const char *username, int totalTime, double totalPrice) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file == NULL) {
        perror("Failed to open log file");
        return;
    }
    fprintf(file, "User: %s, Time Used: %d seconds, Total Price: $%.2f\n", username, totalTime, totalPrice);
    fclose(file);
}

int main() {
    char *content_length = getenv("CONTENT_LENGTH");
    int len = content_length ? atoi(content_length) : 0;
    char post_data[100] = {0};
    char username[50] = "UnknownUser"; // Replace with actual username retrieval logic
    int totalTime = 0;

    // Read POST data
    if (len > 0 && len < sizeof(post_data)) {
        fread(post_data, 1, len, stdin);
    }

    // Parse totalTime from POST data
    char *key = strtok(post_data, "=");
    char *value = strtok(NULL, "=");
    if (key && strcmp(key, "totalTime") == 0) {
        totalTime = atoi(value);
    }

    // Calculate total price
    double totalPrice = totalTime * RATE_PER_SECOND;

    // Log the bill for the admin
    log_bill(username, totalTime, totalPrice);

    // Generate the bill for the user
    printf("Content-Type: text/html\n\n");
    printf("<html><head><title>Bill</title></head><body>");
    printf("<h1>Cyber Cafe Management System</h1>");
    printf("<h2>Bill Summary</h2>");
    printf("<p><strong>User ID:</strong> %s</p>", username);
    printf("<p><strong>Total Time Used:</strong> %d seconds</p>", totalTime);
    printf("<p><strong>Rate:</strong> $%.2f per second</p>", RATE_PER_SECOND);
    printf("<p><strong>Total Price:</strong> $%.2f</p>", totalPrice);
    printf("<button onclick=\"window.location.href='login.html';\">Logout</button>");
    printf("</body></html>");

    return 0;
}
