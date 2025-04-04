#include "header.h"
#include "sqlite3.h" // for db pointer, if needed
#include <ctype.h>    // if you need it

// We'll no longer reference RECORDS or getAccountFromFile
// Instead, we use "sql_create_account" from sql.c


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
    printf("\nSuccess!\n\n");
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
     struct Record r;
     system("clear");
     printf("\t\t\t===== New record =====\n");
 
     // Date input
     while (1) {
         printf("\nEnter today's date (dd/mm/yyyy): ");
         if (scanf("%d/%d/%d", &r.deposit.day, &r.deposit.month, &r.deposit.year) == 3 &&
             r.deposit.day >= 1 && r.deposit.day <= 31 &&
             r.deposit.month >= 1 && r.deposit.month <= 12 &&
             r.deposit.year >= 1900 && r.deposit.year <= 2100) break;
         printf("Invalid date format or values. Use dd/mm/yyyy and valid numbers.\n");
         while (getchar() != '\n');
     }
 
     // Account number
     while (1) {
         printf("\nEnter the account number 8 to 12 digits: ");
         if (scanf("%d", &r.accountNbr) == 1 &&
             r.accountNbr >= 10000000 && r.accountNbr <= 999999999999) break;
         printf("Account number must be numeric and between 8 to 12 digits.\n");
         while (getchar() != '\n');
     }
 
     // Country
     while (1) {
         printf("\nEnter the country (letters only, max 20 characters): ");
         scanf("%s", r.country);
         int valid = 1;
         if (strlen(r.country) > 20) {
             valid = 0;
         }
         for (int i = 0; r.country[i]; i++) {
             if (!isalpha(r.country[i])) {
                 valid = 0;
                 break;
             }
         }
         if (valid) break;
         printf("Invalid country. Only letters allowed and max length 20.\n");
         while (getchar() != '\n');
     }
 
     // Phone
     while (1) {
         printf("\nEnter the phone number (8 digits): ");
         if (scanf("%d", &r.phone) == 1 &&
             r.phone >= 10000000 && r.phone <= 99999999) break;
         printf("Phone number must be exactly 8 digits.\n");
         while (getchar() != '\n');
     }
 
     // Deposit
     while (1) {
         printf("\nEnter amount to deposit (max $500): ");
         if (scanf("%lf", &r.amount) == 1 &&
             r.amount > 0 && r.amount <= 500) break;
         printf("Deposit must be a valid amount up to $500.\n");
         while (getchar() != '\n');
     }
 
     // Account type
     while (1) {
         printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01\n\t-> fixed02\n\t-> fixed03\n\n\tEnter your choice: ");
         scanf("%s", r.accountType);
         if (strcmp(r.accountType, "saving") == 0 ||
             strcmp(r.accountType, "current") == 0 ||
             strcmp(r.accountType, "fixed01") == 0 ||
             strcmp(r.accountType, "fixed02") == 0 ||
             strcmp(r.accountType, "fixed03") == 0) break;
         printf("Invalid account type. Please enter one of the listed options.\n");
         while (getchar() != '\n');
     }
 
     int ok = sql_create_account(u, r);
     if (!ok) {
         printf("Could not create account.\n");
         return;
     }
 
     success(u);
 }
 
 

/**
 * Print all accounts that belong to current user.
 */
 void checkAllAccounts(struct User u)
 {
     system("clear");
     printf("\t\t====== All accounts from user: %s =====\n\n", u.name);
 
     // We just call a function in sql.c that prints or returns all accounts
     int count = sql_select_accounts_for_user(u);
     if (count == 0) {
         printf("No accounts found.\n");
     }
     success(u);
 }
 

/**
 * Update account info (e.g., country or phone).
 */
 void updateAccountInfo(struct User u) {
    int accId;
    printf("Enter account ID to update: ");
    scanf("%d", &accId);

    // Check if account belongs to user:
    // "SELECT account_id FROM Accounts WHERE account_id=accId AND user_id=u.id"
    // if not found, call stayOrReturn(0, updateAccountInfo, u);

    int choice;
    printf("What do you want to update?\n1. Country\n2. Phone\nChoose: ");
    scanf("%d", &choice);

    char query[256];
    char *errMsg = NULL;

    if (choice == 1) {
        char newCountry[50];
        printf("Enter new country: ");
        scanf("%s", newCountry);
        sprintf(query,
          "UPDATE Accounts SET country='%s' WHERE account_id=%d AND user_id=%d;",
          newCountry, accId, u.id
        );
    } else if (choice == 2) {
        int newPhone;
        printf("Enter new phone: ");
        scanf("%d", &newPhone);
        sprintf(query,
          "UPDATE Accounts SET phone=%d WHERE account_id=%d AND user_id=%d;",
          newPhone, accId, u.id
        );
    }

    int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error updating: %s\n", errMsg);
        sqlite3_free(errMsg);
        // handle error
    } else if (sqlite3_changes(db) == 0) {
        printf("Account not found or not yours.\n");
        // handle not found
    } else {
        success(u);
    }
}


/**
 * Check details of a specific account.
 */
 void checkSpecificAccount(struct User u) {
    int accId;
    printf("Enter account ID you want to view: ");
    scanf("%d", &accId);

    char query[256];
    sprintf(query,
      "SELECT date, country, phone, balance, type, account_nbr "
      "FROM Accounts "
      "WHERE account_id=%d AND user_id=%d;", accId, u.id);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        return;
    }
    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = 1;
        const char *date    = (const char*)sqlite3_column_text(stmt, 0);
        const char *country = (const char*)sqlite3_column_text(stmt, 1);
        int phone           = sqlite3_column_int(stmt, 2);
        double balance      = sqlite3_column_double(stmt, 3);
        const char *type    = (const char*)sqlite3_column_text(stmt, 4);
        int accountNbr      = sqlite3_column_int(stmt, 5);

        printf("\nAccount Number: %d\nDate: %s\nCountry: %s\nPhone: %d\nAmount: %.2f\nType: %s\n",
            accountNbr, date, country, phone, balance, type);

        // Calculate interest, etc...
    }
    sqlite3_finalize(stmt);

    if (!found) {
        stayOrReturn(0, checkSpecificAccount, u);
    } else {
        success(u);
    }
}


/**
 * Make deposit or withdrawal on an account (only if not fixed).
 */
 void makeTransaction(struct User u) {
    int accId;
    printf("Enter the account ID to transact with: ");
    scanf("%d", &accId);

    // 1) SELECT the account
    char query[256];
    sprintf(query, 
        "SELECT balance, type FROM Accounts "
        "WHERE account_id=%d AND user_id=%d;", 
        accId, u.id);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(db));
        return;
    }
    double balance = 0;
    char acctType[20];
    int found = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        found = 1;
        balance = sqlite3_column_double(stmt, 0);
        strcpy(acctType, (const char*)sqlite3_column_text(stmt, 1));
    }
    sqlite3_finalize(stmt);

    if (!found) {
        stayOrReturn(0, makeTransaction, u);
        return;
    }
    // 2) if fixed => disallow
    if (strstr(acctType, "fixed") != NULL) {
        printf("Transactions are not allowed on fixed accounts.\n");
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    // 3) deposit or withdraw
    int type;
    double amount;
    printf("Choose transaction type:\n1. Deposit\n2. Withdraw\nYour choice: ");
    scanf("%d", &type);
    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (type == 2 && amount > balance) {
        printf("Not enough balance!\n");
        stayOrReturn(0, makeTransaction, u);
        return;
    }
    double newBal = (type == 1 ? balance + amount : balance - amount);

    // 4) UPDATE the new balance
    sprintf(query, 
        "UPDATE Accounts SET balance=%.2f WHERE account_id=%d AND user_id=%d;",
        newBal, accId, u.id
    );
    char *errMsg=NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error: %s\n", errMsg);
        sqlite3_free(errMsg);
        // handle
    }
    else if (sqlite3_changes(db) == 0) {
        printf("Something failed or account not yours.\n");
        // handle
    } else {
        success(u);
    }
}


/**
 * Delete account by skipping the matching record instead of rewriting it.
 */
 void deleteAccount(struct User u)
 {
     int accountId;
     printf("Enter the account ID to delete: ");
     scanf("%d", &accountId);
 
     // Build query 
     char query[256];
     sprintf(query, 
         "DELETE FROM Accounts WHERE account_id=%d AND user_id=%d;",
         accountId, u.id);
 
     char *errMsg = NULL;
     int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
     if (rc != SQLITE_OK) {
         printf("Error deleting account: %s\n", errMsg);
         sqlite3_free(errMsg);
         // handle
     } else if (sqlite3_changes(db) == 0) {
         printf("No such account or not yours.\n");
         // handle
     } else {
         success(u);
     }
 }
 

/**
 * Transfer ownership by updating the userId and r.name to the new owner.
 */
 void transferOwnership(struct User u)
 {
     int accId;
     printf("Enter the account ID to transfer: ");
     scanf("%d", &accId);
 
     char newOwner[50];
     printf("Enter username to transfer ownership to: ");
     scanf("%s", newOwner);
 
     // 1) Find newOwner in Users
     struct User target;
     memset(&target, 0, sizeof(target));
     strcpy(target.name, newOwner);
     int found = sql_select_user(&target);
     if (!found) {
         printf("Target user does not exist.\n");
         stayOrReturn(0, transferOwnership, u);
         return;
     }
 
     // 2) Update the account
     char query[256];
     sprintf(query,
         "UPDATE Accounts SET user_id=%d "
         "WHERE account_id=%d AND user_id=%d;",
         target.id, accId, u.id
     );
 
     char *errMsg = NULL;
     int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
     if (rc != SQLITE_OK) {
         printf("Error transferring ownership: %s\n", errMsg);
         sqlite3_free(errMsg);
     } else if (sqlite3_changes(db) == 0) {
         printf("No such account or not yours.\n");
         stayOrReturn(0, transferOwnership, u);
         return;
     } else {
         success(u);
     }
 }
 
