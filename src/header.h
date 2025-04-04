#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward-declare sqlite3 pointer
// (Make sure you #include "sqlite3.h" in .c files, not usually in the header.)
extern struct sqlite3 *db; 

struct Date {
    int month, day, year;
};

struct Record {
    int id;
    int userId;
    char accountType[10];
    char name[50];
    char country[50];
    int phone;
    int accountNbr;
    double amount;
    struct Date deposit;
};

struct User {
    int id;
    char name[50];
    char password[50];
};

// auth.c
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// system.c
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void updateAccountInfo(struct User u);
void checkSpecificAccount(struct User u);
void makeTransaction(struct User u);
void deleteAccount(struct User u);
void transferOwnership(struct User u);

// sql.c
void sql_connect(void);
void sql_close(void);

// Insert/select user
int sql_insert_user(struct User *u);
int sql_select_user(struct User *u);  // Fill in id + password if found

// Accounts
int sql_create_account(struct User u, struct Record r);
int sql_select_accounts_for_user(struct User u); 
// etc. for updating, deleting, transferring ownership

#endif
