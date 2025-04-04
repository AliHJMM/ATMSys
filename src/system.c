#include "header.h"
const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
        r.id,
        u.id,
        u.name,
        r.accountNbr,
        r.deposit.month,
        r.deposit.day,
        r.deposit.year,
        r.country,
        r.phone,
        r.amount,
        r.accountType);

}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}


void updateAccountInfo(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    struct Record r;
    char name[100];
    int accId;
    int found = 0;

    printf("Enter account number to update: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r))
    {
        if (strcmp(name, u.name) == 0 && r.accountNbr == accId)
        {
            int choice;
            printf("What do you want to update?\n");
            printf("1. Country\n2. Phone\nChoose: ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                printf("Enter new country: ");
                scanf("%s", r.country);
            }
            else if (choice == 2)
            {
                printf("Enter new phone: ");
                scanf("%d", &r.phone);
            }
            found = 1;
        }
        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id, r.userId, u.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(fp);
    fclose(temp);

    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    if (found)
        success(u);
    else
        stayOrReturn(0, updateAccountInfo, u);
}


void checkSpecificAccount(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    struct Record r;
    char name[100];
    int accId, found = 0;

    printf("Enter the account number you want to view: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r))
    {
        if (strcmp(name, u.name) == 0 && r.accountNbr == accId)
        {
            found = 1;
            printf("\nAccount Number: %d\nDate: %d/%d/%d\nCountry: %s\nPhone: %d\nAmount: %.2f\nType: %s\n",
                   r.accountNbr,
                   r.deposit.day, r.deposit.month, r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);

            double interest = 0;
            if (strcmp(r.accountType, "saving") == 0)
                interest = r.amount * 0.07;
            else if (strcmp(r.accountType, "fixed01") == 0)
                interest = r.amount * 0.04;
            else if (strcmp(r.accountType, "fixed02") == 0)
                interest = r.amount * 0.05;
            else if (strcmp(r.accountType, "fixed03") == 0)
                interest = r.amount * 0.08;

            if (strcmp(r.accountType, "current") == 0)
            {
                printf("Note: You will not get interests because the account is of type current.\n");
            }
            else
            {
                printf("You will get $%.2f as interest on day %d of every month.\n", interest, r.deposit.day);
            }
        }
    }

    fclose(fp);
    if (found)
        success(u);
    else
        stayOrReturn(0, checkSpecificAccount, u);
}


void makeTransaction(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    struct Record r;
    char name[100];
    int accId, found = 0;

    printf("Enter the account number to transact with: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r))
    {
        if (strcmp(name, u.name) == 0 && r.accountNbr == accId)
        {
            found = 1;

            if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0)
            {
                printf("✖ Transactions are not allowed on fixed accounts.\n");
                fclose(fp);
                fclose(temp);
                remove("./data/temp.txt");
                stayOrReturn(0, makeTransaction, u);
                return;
            }

            int type;
            double amount;

            printf("Choose transaction type:\n1. Deposit\n2. Withdraw\nYour choice: ");
            scanf("%d", &type);
            printf("Enter amount: ");
            scanf("%lf", &amount);

            if (type == 1)
            {
                r.amount += amount;
            }
            else if (type == 2)
            {
                if (amount > r.amount)
                {
                    printf("✖ Not enough balance!\n");
                    fclose(fp);
                    fclose(temp);
                    remove("./data/temp.txt");
                    stayOrReturn(0, makeTransaction, u);
                    return;
                }
                r.amount -= amount;
            }
        }

        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id, r.userId, u.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(fp);
    fclose(temp);

    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    if (found)
        success(u);
    else
        stayOrReturn(0, makeTransaction, u);
}


void deleteAccount(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    struct Record r;
    char name[100];
    int accId, found = 0;

    printf("Enter the account number to delete: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r))
    {
        if (strcmp(name, u.name) == 0 && r.accountNbr == accId)
        {
            found = 1;
        }

        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id, r.userId, name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(fp);
    fclose(temp);

    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    if (found)
        success(u);
    else
        stayOrReturn(0, deleteAccount, u);
}


void transferOwnership(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    struct Record r;
    char name[100];
    int accId, found = 0;
    char newOwner[50];
    int newUserId = -1;

    printf("Enter the account number to transfer: ");
    scanf("%d", &accId);

    printf("Enter username to transfer ownership to: ");
    scanf("%s", newOwner);

    FILE *userFile = fopen(USERS, "r");
    struct User uTemp;
    while (fscanf(userFile, "%d %s %s", &uTemp.id, uTemp.name, uTemp.password) != EOF)
    {
        if (strcmp(uTemp.name, newOwner) == 0)
        {
            newUserId = uTemp.id;
            break;
        }
    }
    fclose(userFile);

    if (newUserId == -1)
    {
        printf("✖ Target user does not exist.\n");
        fclose(fp);
        fclose(temp);
        remove("./data/temp.txt");
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    while (getAccountFromFile(fp, name, &r))
    {
        if (strcmp(name, u.name) == 0 && r.accountNbr == accId)
        {
            found = 1;
            r.userId = newUserId;
            strcpy(name, newOwner);
        }

        fprintf(temp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                r.id, r.userId, name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(fp);
    fclose(temp);

    remove(RECORDS);
    rename("./data/temp.txt", RECORDS);

    if (found)
        success(u);
    else
        stayOrReturn(0, transferOwnership, u);
}
