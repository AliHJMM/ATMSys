#include "header.h"

const char *RECORDS = "./data/records.txt";

/**
 * Correctly reads one line (9 space-separated fields) from records.txt:
 * Format: 
 *   id userId name accountNbr date country phone amount accountType
 * Example line:
 *   0 0 Alice  0 10/10/2012 Africa 291321234 22432.52 saving
 */
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    char dateStr[20];

    // Read 9 fields; treat the entire "10/10/2012" as dateStr
    int fields = fscanf(ptr, "%d %d %s %d %s %s %d %lf %s",
        &r->id,
        &r->userId,
        r->name,          // we can store directly in r->name
        &r->accountNbr,
        dateStr,          // "10/10/2012"
        r->country,
        &r->phone,
        &r->amount,
        r->accountType
    );

    if (fields != 9) {
        // Either end of file or a malformed line.
        return 0;
    }

    // Convert "10/10/2012" into r->deposit.month/day/year
    if (sscanf(dateStr, "%d/%d/%d",
               &r->deposit.month,
               &r->deposit.day,
               &r->deposit.year) != 3)
    {
        // Date not in expected format
        return 0;
    }

    // For convenience, also copy r->name outward if needed
    strcpy(name, r->name);
    return 1;
}

/**
 * Writes one record (9 fields) to the file in the same format used by getAccountFromFile().
 */
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    // Example line:
    //   0 0 Alice  0 10/10/2012 Africa 291321234 22432.52 saving
    fprintf(ptr, "%d %d %s %d %02d/%02d/%04d %s %d %.2f %s\n",
        r.id,
        u.id,         // the user ID of the record
        u.name,       // the username
        r.accountNbr,
        r.deposit.month,
        r.deposit.day,
        r.deposit.year,
        r.country,
        r.phone,
        r.amount,
        r.accountType
    );
}

/**
 * Helper to let user retry or return to main menu.
 */
void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0) {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit: ");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    } else {
        printf("\nEnter 1 to go to the main menu and 0 to exit: ");
        scanf("%d", &option);
    }
    if (option == 1) {
        system("clear");
        mainMenu(u);
    } else {
        system("clear");
        exit(1);
    }
}

/**
 * After any successful operation, ask user whether to exit or go to menu.
 */
void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1) {
        mainMenu(u);
    } else if (option == 0) {
        exit(1);
    } else {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

/**
 * Create new account for current user.
 */
void createNewAcc(struct User u)
{
    struct Record r, cr;
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf) {
        printf("Cannot open %s\n", RECORDS);
        exit(1);
    }

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date (mm/dd/yyyy): ");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNbr);

    // Rewind to check that this account doesn't already exist for this user
    rewind(pf);
    char dummy[50]; 
    while (getAccountFromFile(pf, dummy, &cr)) {
        if (strcmp(cr.name, u.name) == 0 && cr.accountNbr == r.accountNbr) {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }

    // Fill in rest
    printf("\nEnter the country: ");
    scanf("%s", r.country);
    printf("\nEnter the phone number: ");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01\n\t-> fixed02\n\t-> fixed03\n\n\tEnter your choice: ");
    scanf("%s", r.accountType);

    // Just set a new ID, or reuse zero if you prefer
    // (You can do something like r.id = someAutoIncrement if desired.)
    r.id = 0; 

    // Save to file
    saveAccountToFile(pf, u, r);
    fclose(pf);

    success(u);
}

/**
 * Print all accounts that belong to current user.
 */
void checkAllAccounts(struct User u)
{
    FILE *pf = fopen(RECORDS, "r");
    if (!pf) {
        printf("Cannot open %s\n", RECORDS);
        exit(1);
    }

    system("clear");
    printf("\t\t====== All accounts from user: %s =====\n\n", u.name);

    struct Record r;
    char dummy[50];
    int any = 0;
    while (getAccountFromFile(pf, dummy, &r)) {
        // belongs to user?
        if (strcmp(r.name, u.name) == 0) {
            any = 1;
            printf("_____________________\n");
            printf("Account number: %d\nDeposit Date: %02d/%02d/%04d\nCountry: %s\n"
                   "Phone: %d\nAmount: $%.2f\nType: %s\n",
                   r.accountNbr,
                   r.deposit.month, r.deposit.day, r.deposit.year,
                   r.country, r.phone, r.amount, r.accountType);
        }
    }
    fclose(pf);

    if (!any) {
        printf("No accounts found.\n");
    }
    success(u);
}

/**
 * Update account info (e.g., country or phone).
 */
void updateAccountInfo(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Cannot open files!\n");
        exit(1);
    }

    struct Record r;
    char name[50];
    int accId, found = 0;

    printf("Enter account number to update: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r)) {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accId) {
            int choice;
            printf("What do you want to update?\n");
            printf("1. Country\n2. Phone\nChoose: ");
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter new country: ");
                scanf("%s", r.country);
            } else if (choice == 2) {
                printf("Enter new phone: ");
                scanf("%d", &r.phone);
            }
            found = 1;
        }
        // Always write the (possibly updated) record
        fprintf(temp, "%d %d %s %d %02d/%02d/%04d %s %d %.2f %s\n",
            r.id,
            r.userId,
            r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
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

/**
 * Check details of a specific account.
 */
void checkSpecificAccount(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    if (!fp) {
        printf("Cannot open %s\n", RECORDS);
        exit(1);
    }

    struct Record r;
    char name[50];
    int accId, found = 0;

    printf("Enter the account number you want to view: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r)) {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accId) {
            found = 1;
            printf("\nAccount Number: %d\nDate: %d/%d/%d\nCountry: %s\nPhone: %d\nAmount: %.2f\nType: %s\n",
                   r.accountNbr,
                   r.deposit.month, r.deposit.day, r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);

            // Show interest (similar to the working code)
            double interest = 0;
            if (strcmp(r.accountType, "saving") == 0)
                interest = r.amount * 0.07;
            else if (strcmp(r.accountType, "fixed01") == 0)
                interest = r.amount * 0.04;
            else if (strcmp(r.accountType, "fixed02") == 0)
                interest = r.amount * 0.05;
            else if (strcmp(r.accountType, "fixed03") == 0)
                interest = r.amount * 0.08;

            if (strcmp(r.accountType, "current") == 0) {
                printf("Note: No interest for current accounts.\n");
            } else {
                printf("Potential interest: $%.2f\n", interest);
            }
        }
    }

    fclose(fp);
    if (found)
        success(u);
    else
        stayOrReturn(0, checkSpecificAccount, u);
}

/**
 * Make deposit or withdrawal on an account (only if not fixed).
 */
void makeTransaction(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Cannot open files!\n");
        exit(1);
    }

    struct Record r;
    char name[50];
    int accId, found = 0;

    printf("Enter the account number to transact with: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r)) {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accId) {
            found = 1;
            // Disallow transaction on fixed accounts
            if (strstr(r.accountType, "fixed") != NULL) {
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

            if (type == 1) {
                r.amount += amount;
            } else if (type == 2) {
                if (amount > r.amount) {
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
        // Rewrite record (possibly updated)
        fprintf(temp, "%d %d %s %d %02d/%02d/%04d %s %d %.2f %s\n",
            r.id,
            r.userId,
            r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
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

/**
 * Delete account by skipping the matching record instead of rewriting it.
 */
void deleteAccount(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Cannot open files!\n");
        exit(1);
    }

    struct Record r;
    char name[50];
    int accId, found = 0;

    printf("Enter the account number to delete: ");
    scanf("%d", &accId);

    while (getAccountFromFile(fp, name, &r)) {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accId) {
            found = 1;
            // Skip writing -> effectively deletes it
            continue;
        }
        // Write everything else
        fprintf(temp, "%d %d %s %d %02d/%02d/%04d %s %d %.2f %s\n",
            r.id,
            r.userId,
            r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
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

/**
 * Transfer ownership by updating the userId and r.name to the new owner.
 */
void transferOwnership(struct User u)
{
    FILE *fp = fopen(RECORDS, "r");
    FILE *temp = fopen("./data/temp.txt", "w");
    if (!fp || !temp) {
        printf("Cannot open files!\n");
        exit(1);
    }

    struct Record r;
    char name[50];
    int accId, found = 0;
    char newOwner[50];
    int newUserId = -1;

    printf("Enter the account number to transfer: ");
    scanf("%d", &accId);

    printf("Enter username to transfer ownership to: ");
    scanf("%s", newOwner);

    // Find the newOwner’s ID from users.txt
    FILE *userFile = fopen(USERS, "r");
    if (!userFile) {
        printf("Cannot open %s\n", USERS);
        exit(1);
    }

    struct User uTemp;
    while (fscanf(userFile, "%d %s %s", &uTemp.id, uTemp.name, uTemp.password) != EOF) {
        if (strcmp(uTemp.name, newOwner) == 0) {
            newUserId = uTemp.id;
            break;
        }
    }
    fclose(userFile);

    if (newUserId == -1) {
        printf("✖ Target user does not exist.\n");
        fclose(fp);
        fclose(temp);
        remove("./data/temp.txt");
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    while (getAccountFromFile(fp, name, &r)) {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accId) {
            found = 1;
            r.userId = newUserId;
            strcpy(r.name, newOwner); // rename the record
        }
        fprintf(temp, "%d %d %s %d %02d/%02d/%04d %s %d %.2f %s\n",
            r.id,
            r.userId,
            r.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
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
