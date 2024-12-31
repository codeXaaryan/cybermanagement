#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to fetch and display user data from the CSV file
void fetch_users_from_csv(const char *database_csv) {
    FILE *file = fopen(database_csv, "r");
    if (file == NULL) {
        printf("<tr><td colspan=\"3\">Error: Unable to open database file.</td></tr>");
        return;
    }

    char line[1024];
    int first_line = 1;  // To skip the header row if needed
    while (fgets(line, sizeof(line), file)) {
        if (first_line) {  // Skip the header line if necessary
            first_line = 0;
            continue;
        }
        
        // Tokenize the CSV line by commas
        char *name = strtok(line, ",");
        char *email = strtok(NULL, ",");
        char *username = strtok(NULL, ",");

        // Remove trailing newline from username if present
        if (username) {
            username[strcspn(username, "\n")] = '\0';
        }

        // Check if the data is valid and then print it in a table row
        if (name && email && username) {
            printf("<tr><td>%s</td><td>%s</td><td>%s</td></tr>", name, email, username);
        }
    }

    fclose(file);
}

int main() {
    // Output HTTP content-type header for HTML
    printf("Content-type: text/html\n\n");

    // Output the start of the HTML page
    printf("<!DOCTYPE html>");
    printf("<html lang=\"en\">");
    printf("<head>");
    printf("<meta charset=\"UTF-8\">");
    printf("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    printf("<title>Manage Users - Cyber Cafe Management System</title>");
    printf("<style>");
    // Styles for the page
    printf("body { font-family: Arial, sans-serif; background-color: #f4f7fa; margin: 0; padding: 0; }");
    printf("header { background-color: #007bff; color: white; padding: 15px; text-align: center; font-size: 24px; }");
    printf("main { margin: 30px; background: white; padding: 20px; box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1); }");
    printf("h1 { text-align: center; font-size: 28px; margin-bottom: 20px; color: #333; }");
    printf("table { width: 100%%; border-collapse: collapse; margin-bottom: 20px; }");
    printf("th, td { padding: 10px; text-align: left; border: 1px solid #ddd; }");
    printf("th { background-color: #007bff; color: white; }");
    printf("button { padding: 12px 20px; font-size: 16px; background-color: #28a745; color: white; border: none; border-radius: 5px; cursor: pointer; width: 200px; text-align: center; }");
    printf("button:hover { background-color: #218838; }");
    printf(".action-buttons { text-align: center; }");
    printf("</style>");
    printf("</head>");
    printf("<body>");

    // Header
    printf("<header><h2>Manage Users - Cyber Cafe Management System</h2></header>");
    printf("<main><h1>List of Users</h1>");

    // Table to display user data
    printf("<table>");
    printf("<tr><th>Name</th><th>Email</th><th>Username</th></tr>");

    // Fetch and display user data from CSV
    fetch_users_from_csv("/path/to/cgi-bin/database.csv");  // Update path to your CSV file

    // Close the table
    printf("</table>");

    // Button to add a new user
    printf("<div class=\"action-buttons\"><button><a href=\"add_user.html\" style=\"color: white; text-decoration: none;\">Add New User</a></button></div>");
    printf("</main>");

    // Close the HTML tags
    printf("</body>");
    printf("</html>");

    return 0;
}
