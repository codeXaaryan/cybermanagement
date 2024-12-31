#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100
#define USER_FILE "users.csv" // Assuming the user data is stored in a CSV file

// Function to update user status
void update_user_status(const char *username, const char *action) {
    FILE *file = fopen(USER_FILE, "r+");
    if (file == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Could not open user file.</h3></body></html>");
        exit(1);
    }

    char line[MAX_INPUT_SIZE];
    char new_lines[100][MAX_INPUT_SIZE]; // To store modified user data
    int line_count = 0;

    while (fgets(line, sizeof(line), file)) {
        char user[MAX_INPUT_SIZE], status[MAX_INPUT_SIZE];
        sscanf(line, "%[^,],%s", user, status);

        if (strcmp(user, username) == 0) {
            // Update the status of the user
            if (strcmp(action, "disable") == 0) {
                strcpy(status, "disabled");
            } else if (strcmp(action, "enable") == 0) {
                strcpy(status, "enabled");
            }
        }

        // Save modified line back to array
        sprintf(new_lines[line_count], "%s,%s", user, status);
        line_count++;
    }

    // Write updated data back to the file
    freopen(USER_FILE, "w", file);
    for (int i = 0; i < line_count; i++) {
        fprintf(file, "%s\n", new_lines[i]);
    }

    fclose(file);

    // Return confirmation to frontend
    printf("Content-Type: text/html\n\n");
    printf("<html><body><h3>User status updated successfully!</h3></body></html>");
}

int main() {
    char *username = getenv("QUERY_STRING");
    char *action = strstr(username, "action=") + 7; // Extract action parameter
    username = strtok(username, "&"); // Extract username parameter

    update_user_status(username, action); // Update user status based on action
    return 0;
}
