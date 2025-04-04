#include "header.h"

const char *USERS = "./data/users.txt";


void loginMenu(char a[50], char pass[50])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n");
    printf("\t\t\t\t\t User Login:\n");
    printf("Enter username: ");
    fflush(stdout);  // Ensure the prompt is printed immediately
    scanf("%s", a);

    printf("Enter the password to login: ");
    fflush(stdout);
    scanf("%s", pass);
}



const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
{
    if (strcmp(userChecker.name, u.name) == 0)
    {
        fclose(fp);
        return strdup(userChecker.password);
    }
}


    fclose(fp);
    return "no user found";
}

void registerMenu(char a[50], char pass[50])
{
    FILE *fp;
    struct User userChecker;
    int exists = 0;
    int lastId = -1;

    fp = fopen(USERS, "r");
    if (fp != NULL)
    {
        while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
        {
            lastId = userChecker.id;
            if (strcmp(userChecker.name, a) == 0)
            {
                exists = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (exists)
    {
        printf("✖ User already exists! Try another name.\n");
        exit(1);
    }

    // Ask for password input
    printf("Enter a password for registration: ");
    fflush(stdout);
    scanf("%s", pass);

    FILE *fw = fopen(USERS, "a");
    fprintf(fw, "%d %s %s\n", lastId + 1, a, pass);
    fclose(fw);

    printf("✔ Registration successful! Welcome, %s\n", a);
}
