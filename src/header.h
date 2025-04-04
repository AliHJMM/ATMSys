#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

extern const char *USERS;
extern const char *RECORDS;

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

#endif
