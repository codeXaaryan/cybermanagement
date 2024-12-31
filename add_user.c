#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1000

// Function to parse POST data and extract field values
void parse_post_data(char *post_data, char *name, char *phone_email, char *csv_username, char *csv_password, char *role) {
    char *key, *value;
    key = strtok(post_data, "&");
    while (key != NULL) {
        value = strchr(key, '=');
        if (value != NULL) {
            *value = '\0'; // Split key and value
            value++;
            // Decode URL-encoded characters (e.g., + to space)
            for (char *p = value; *p; ++p) {
                if (*p == '+') {
                    *p = ' ';
                }
            }
            if (strcmp(key, "name") == 0) {
                strncpy(name, value, MAX_INPUT_SIZE - 1);
            } else if (strcmp(key, "phone_email") == 0) {
                strncpy(phone_email, value, MAX_INPUT_SIZE - 1);
            } else if (strcmp(key, "username") == 0) {
                strncpy(csv_username, value, MAX_INPUT_SIZE - 1);
            } else if (strcmp(key, "password") == 0) {
                strncpy(csv_password, value, MAX_INPUT_SIZE - 1);
            } else if (strcmp(key, "role") == 0) {
                strncpy(role, value, MAX_INPUT_SIZE - 1);
            }
        }
        key = strtok(NULL, "&");
    }
}

// Function to save user data to the CSV file
void save_to_csv(const char *name, const char *phone_email, const char *csv_username, const char *csv_password, const char *role) {
    // Adjust the file path as per your setup
    const char *file_path = "database.csv"; // Update this path if needed

    FILE *file = fopen(file_path, "a");
    if (file == NULL) {
        // Enhanced error reporting
        perror("Error opening file");
        printf("Content-Type: text/html\n\n");
        printf("<html><body>");
        printf("<h3>Error: Could not open the database file at '%s'.</h3>", file_path);
        printf("</body></html>");
        exit(1);
    }

    // Save user data in CSV format with correct column order
    fprintf(file, "%s,%s,%s,%s,%s\n", name, phone_email, csv_username, csv_password, role);
    fclose(file);
}

int main() {
    // Get the content length from the environment variable
    char *content_length = getenv("CONTENT_LENGTH");
    int len = content_length ? atoi(content_length) : 0;

    if (len <= 0 || len >= MAX_INPUT_SIZE) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Invalid content length.</h3></body></html>");
        return 1;
    }

    // Read the POST data from stdin
    char post_data[MAX_INPUT_SIZE] = {0};
    fread(post_data, 1, len, stdin);

    // Buffers to store user input
    char name[MAX_INPUT_SIZE] = {0};
    char phone_email[MAX_INPUT_SIZE] = {0};
    char csv_username[MAX_INPUT_SIZE] = {0};
    char csv_password[MAX_INPUT_SIZE] = {0};
    char role[MAX_INPUT_SIZE] = {0};

    // Parse the POST data
    parse_post_data(post_data, name, phone_email, csv_username, csv_password, role);

    // Save user data to the CSV file
    save_to_csv(name, phone_email, csv_username, csv_password, role);

    // Provide feedback to the user
    printf("Content-Type: text/html\n\n");
    printf("<html>");
    printf("<head>");
    printf("<title>User Added Successfully</title>");
    printf("<style>");
    printf("body { font-family: Arial, sans-serif; background-color: #f0f8ff; text-align: center; padding: 50px; }");
    printf(".success-container { background-color: #d4edda; border: 1px solid #c3e6cb; padding: 20px; border-radius: 5px; display: inline-block; }");
    printf(".success-container h1 { color: #155724; }");
    printf("a { color: #007bff; text-decoration: none; font-weight: bold; }");
    printf("</style>");
    printf("<meta http-equiv='refresh' content='5;url=/manage_users.html'>"); // Redirect after 5 seconds
    printf("</head>");
    printf("<body>");
    printf("<div class='success-container'>");
    printf("<h1>User Added Successfully!</h1>");
    printf("<p>The user <strong>%s</strong> has been added to the system.</p>", csv_username);
    printf("<p>You will be redirected to the <a href='/manage_users.html'>Manage Users</a> page in 5 seconds.</p>");
    printf("</div>");
    printf("</body>");
    printf("</html>");

    return 0;
}
