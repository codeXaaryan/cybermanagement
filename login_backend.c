#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100
#define MAX_LINE_LENGTH 256

// URL-decode function
void url_decode(char *str) {
    char *pos = str;
    int i = 0;
    while (str[i]) {
        if (str[i] == '%' && str[i + 1] && str[i + 2]) {
            int hex_value;
            sscanf(&str[i + 1], "%2x", &hex_value);
            str[i] = (char)hex_value;
            memmove(&str[i + 1], &str[i + 3], strlen(&str[i + 3]) + 1);
        }
        i++;
    }
}

void parse_post_data(char *post_data, char *username, char *password) {
    char *key, *value;
    key = strtok(post_data, "&");
    while (key != NULL) {
        value = strchr(key, '=');
        if (value != NULL) {
            *value = '\0'; // Split key and value
            value++;
            if (strcmp(key, "first") == 0) {
                strncpy(username, value, MAX_INPUT_SIZE);
            } else if (strcmp(key, "password") == 0) {
                strncpy(password, value, MAX_INPUT_SIZE);
                url_decode(password);  // Decode the password here
            }
        }
        key = strtok(NULL, "&");
    }
}

int check_credentials(const char *username, const char *password, char *role) {
    FILE *file = fopen("database.csv", "r");
    if (file == NULL) {
        return 0;
    }
    char line[MAX_LINE_LENGTH];
    char name[MAX_INPUT_SIZE], phone_email[MAX_INPUT_SIZE];
    char csv_username[MAX_INPUT_SIZE], csv_password[MAX_INPUT_SIZE], csv_role[MAX_INPUT_SIZE];

    fgets(line, sizeof(line), file); // Skip the header row

    while (fgets(line, sizeof(line), file)) {
        // Update to parse the new CSV structure
        if (sscanf(line, "%99[^,],%99[^,],%99[^,],%99[^,],%99s",
                   name, phone_email, csv_username, csv_password, csv_role) == 5) {
            if (strcmp(username, csv_username) == 0 && strcmp(password, csv_password) == 0) {
                strncpy(role, csv_role, MAX_INPUT_SIZE);
                fclose(file);
                return 1;  // Credentials match
            }
        }
    }

    fclose(file);
    return 0;  // Credentials do not match
}

int main() {
    char *content_length = getenv("CONTENT_LENGTH");
    int len = content_length ? atoi(content_length) : 0;
    char post_data[MAX_LINE_LENGTH] = {0};
    char username[MAX_INPUT_SIZE] = {0};
    char password[MAX_INPUT_SIZE] = {0};
    char role[MAX_INPUT_SIZE] = {0};

    if (len > 0 && len < sizeof(post_data)) {
        fread(post_data, 1, len, stdin);
        parse_post_data(post_data, username, password);
    }

    printf("Content-Type: text/html\n\n");
    printf("<html><head><title>Login Result</title></head><body>");
    printf("<h1>Cyber Cafe Management System</h1>");

    if (check_credentials(username, password, role)) {
        // Check the role
        if (strcmp(role, "admin") == 0) {
            // Redirect to the admin page if the user is an admin
            printf("<script>window.location.href = '/admin.html';</script>");
        } else {
            // Normal user login successful
            printf("<h3>Login Successful</h3>");
            printf("<script>window.location.href = '/service.html';</script>");
        }
    } else {
        printf("<h3>Login Failed</h3>");
        printf("<p>Invalid username or password. Please try again.</p>");
    }

    printf("<a href='login_page.html'>Go back to login</a>");
    printf("</body></html>");

    return 0;
}
