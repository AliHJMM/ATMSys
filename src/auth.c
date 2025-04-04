#include "header.h"
#include "sqlite3.h" // if needed

// No longer needed
// const char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n");
    printf("\t\t\t\t\t User Login:\n");
    printf("Enter username: ");
    fflush(stdout);
    scanf("%s", a);

    printf("Enter the password to login: ");
    fflush(stdout);
    scanf("%s", pass);
}

/**
 * Instead of scanning users.txt, we call sql_select_user()
 */
const char *getPassword(struct User u)
{
    // We'll attempt to find the user in the DB
    struct User tmp;
    strcpy(tmp.name, u.name);

    int found = sql_select_user(&tmp);
    if (!found) {
        return "no user found";
    }
    return strdup(tmp.password);
}

/**
 * Instead of writing to users.txt, we call sql_insert_user().
 */
void registerMenu(char a[50], char pass[50])
{
    printf("Enter a password for registration: ");
    fflush(stdout);
    scanf("%s", pass);

    struct User newU;
    newU.id = 0;
    strcpy(newU.name, a);
    strcpy(newU.password, pass);

    int ret = sql_insert_user(&newU);
    if (ret == 0) {
        printf("✖ User already exists! Try another name.\n");
        exit(1);
    } else if (ret == -1) {
        printf("✖ Some DB error occurred.\n");
        exit(1);
    } else {
        printf("✔ Registration successful! Welcome, %s\n", a);
    }
}
