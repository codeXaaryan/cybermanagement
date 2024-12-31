#include <stdio.h>
#include <stdlib.h>

void print_users() {
    FILE *file = fopen("database.csv", "r");
    if (file == NULL) {
        printf("Content-Type: text/html\n\n");
        printf("<html><body><h3>Error: Could not open users file.</h3></body></html>");
        exit(1);
    }
    
    char name[100], email[100], username[100];
    printf("Content-Type: text/html\n\n");
    printf("<html><body><h2>User List</h2><table>");
    printf("<tr><th>Name</th><th>Email</th><th>Username</th></tr>");
    
    while (fscanf(file, "%99[^,],%99[^,],%99[^\n]\n", name, email, username) == 3) {
        printf("<tr><td>%s</td><td>%s</td><td>%s</td></tr>", name, email, username);
    }

    printf("</table></body></html>");
    fclose(file);
}

int main() {
    print_users();
    return 0;
}
