#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100
#define MAX_LINE_LENGTH 256

void get_input(char *input_buffer, int size) {
    fgets(input_buffer, size, stdin);
    input_buffer[strcspn(input_buffer, "\n")] = '\0';  // Remove newline character
}

void save_to_csv(const char *name, const char *phone_email, const char *username, const char *password) {
    FILE *file = fopen("database.csv", "a");
    if (file == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Could not open the database file.</h3></body></html>");
        exit(1);
    }

    // Write user data to CSV file
    fprintf(file, "%s,%s,%s,%s\n", name, phone_email, username, password);
    fclose(file);
}

int check_passwords(const char *password, const char *confirm_password) {
    return strcmp(password, confirm_password) == 0;
}

int main() {
    char name[MAX_INPUT_SIZE];
    char phone_email[MAX_INPUT_SIZE];
    char username[MAX_INPUT_SIZE];
    char password[MAX_INPUT_SIZE];
    char confirm_password[MAX_INPUT_SIZE];

    printf("Content-Type: text/html\n\n");

    // Collecting form data from user
    get_input(name, MAX_INPUT_SIZE);            
    get_input(phone_email, MAX_INPUT_SIZE);    
    get_input(username, MAX_INPUT_SIZE);       
    get_input(password, MAX_INPUT_SIZE);       
    get_input(confirm_password, MAX_INPUT_SIZE); 

    // Displaying HTML output
    printf("<html><head><title>Registration Result</title></head><body>");
    printf("<h1>Cyber Cafe Management System</h1>");

    // Check if passwords match
    if (!check_passwords(password, confirm_password)) {
        printf("<h3>Registration Failed</h3>");
        printf("<p>Passwords do not match. Please try again.</p>");
        printf("<a href='/registration.html'>Go back to registration page</a>");
    } else {
        // Save user data to CSV file
        save_to_csv(name, phone_email, username, password);
        printf("<h3>Registration Successful!</h3>");
        printf("<p>Welcome, %s! Your registration is complete.</p>", username);
        printf("<a href='/login.html'>Go to login page</a>");
    }

    printf("</body></html>");
    return 0;
}
