#include "header.h"
#include <ctype.h>


// Forward declaration
void initMenu(struct User *u);

void mainMenu(struct User u)
{
    int option;
    while (1)
    {
        system("clear");
        printf("\n\n\t\t======= ATM =======\n\n");
        printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
        printf("\n\t\t[1]- Create a new account\n");
        printf("\n\t\t[2]- Update account information\n");
        printf("\n\t\t[3]- Check accounts\n");
        printf("\n\t\t[4]- Check list of owned account\n");
        printf("\n\t\t[5]- Make Transaction\n");
        printf("\n\t\t[6]- Remove existing account\n");
        printf("\n\t\t[7]- Transfer ownership\n");
        printf("\n\t\t[8]- Exit\n");
        printf("\nEnter your choice: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n'); // clear invalid input
            printf("\nInvalid input! Please enter a number between 1 and 8.\n");
            printf("Press Enter to try again...");
            getchar();
            continue;
        }

        switch (option)
        {
        case 1: createNewAcc(u); break;
        case 2: updateAccountInfo(u); break;
        case 3: checkSpecificAccount(u); break;
        case 4: checkAllAccounts(u); break;
        case 5: makeTransaction(u); break;
        case 6: deleteAccount(u); break;
        case 7: transferOwnership(u); break;
        case 8:
            sql_close();
            exit(0);
        default:
            printf("\nInvalid choice! Please enter a number between 1 and 8.\n");
            printf("Press Enter to try again...");
            while (getchar() != '\n'); // clear buffer
            getchar(); // wait for enter
            break;
        }
    }
}



void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        printf("\nChoose an option: [1] Login  [2] Register  [3] Exit\n> ");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            {
                const char* pw = getPassword(*u); // "no user found" or actual pass
                if (strcmp(u->password, pw) == 0)
                {
                    printf("\n\n Password Match!\n");
                }
                else
                {
                    printf("\nIncorrect username or password. Please try again.\n");
                    sql_close();
                    exit(1);
                }
                r = 1;
            }
            break;
        case 2:
        while (1) {
            printf("Enter new username: ");
            scanf("%s", u->name);
        
            if (strlen(u->name) > 20 || strlen(u->name) == 0) {
                printf("Username must be 1–20 characters.\n");
                continue;
            }
            int valid = 1;
            for (int i = 0; i < strlen(u->name); i++) {
                if (!isalpha(u->name[i])) {
                    valid = 0;
                    break;
                }
            }
            if (!valid) {
                printf("Username must contain only letters (A-Z, a-z).\n");
                continue;
            }
        
            struct User temp;
            strcpy(temp.name, u->name);
            if (sql_select_user(&temp)) {
                printf("Username already exists! Try another one.\n");
                continue;
            }
            break;
        }
        
        registerMenu(u->name, u->password);
        r = 1;
        
            break;
        case 3:
            sql_close();
            exit(1);
        default:
            printf("Invalid option. Please choose 1, 2, or 3.\n");
        }
    }

};

int main()
{
    // 1) connect to DB
    sql_connect();

    struct User u;
    u.id = 0;
    strcpy(u.name, "");
    strcpy(u.password, "");

    // 2) do the initMenu + mainMenu
    initMenu(&u);
    mainMenu(u);

    // If user never chooses exit, close DB before returning
    sql_close();
    return 0;
}
