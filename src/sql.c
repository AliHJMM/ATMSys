#include "sqlite3.h"
#include "header.h"

sqlite3 *db = NULL;  // global pointer to the DB connection

/**
 * Connect to the database file "db/atm.db". 
 * Also create the needed tables if they don't exist.
 */
void sql_connect(void) {
    int rc = sqlite3_open("./db/atm.db", &db);
    if (rc != SQLITE_OK) {
        printf("❌ Cannot open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    // Create tables if not exist
    const char *sql_users =
        "CREATE TABLE IF NOT EXISTS Users ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name TEXT UNIQUE NOT NULL,"
        "  password TEXT NOT NULL"
        ");";

    const char *sql_accounts =
        "CREATE TABLE IF NOT EXISTS Accounts ("
        "  account_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  user_id INTEGER NOT NULL,"
        "  date TEXT,"
        "  country TEXT,"
        "  phone INTEGER,"
        "  balance REAL,"
        "  type TEXT,"
        "  account_nbr INTEGER,"
        "  FOREIGN KEY(user_id) REFERENCES Users(id)"
        ");";

    char *errMsg = NULL;
    rc = sqlite3_exec(db, sql_users, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error creating Users table: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    rc = sqlite3_exec(db, sql_accounts, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error creating Accounts table: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

/**
 * Close the DB when the program ends.
 */
void sql_close(void) {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}

/**
 * Insert a new user. Return:
 *   1 on success,
 *   0 if user name already exists,
 *  -1 on other error
 */
 int sql_insert_user(struct User *u) {
    printf("[DEBUG] Inserting user: %s with password: %s\n", u->name, u->password);  // ADD THIS LINE

    const char *query = "INSERT INTO Users(name, password) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Prepare error (insert user): %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, u->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, u->password, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Insert step error: %s\n", sqlite3_errmsg(db));  // ADD THIS LINE
        if (rc == SQLITE_CONSTRAINT) {
            sqlite3_finalize(stmt);
            return 0;
        } else {
            sqlite3_finalize(stmt);
            return -1;
        }
    }

    sqlite3_finalize(stmt);
    return 1;
}



/**
 * Look up user by name. If found, fill in user->id and user->password. Return 1. 
 * If not found, return 0.
 */
 int sql_select_user(struct User *u) {
    char query[256];
    sprintf(query, 
        "SELECT id, password FROM Users WHERE name='%s';", u->name);
    
    sqlite3_stmt *stmt;
    printf("[DEBUG] Looking up user: %s\n", u->name);  // Added debug

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL prepare error (select user): %s\n", sqlite3_errmsg(db));
        return 0;
    }

    int ret = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        u->id = sqlite3_column_int(stmt, 0);
        const unsigned char *pw = sqlite3_column_text(stmt, 1);
        strcpy(u->password, (const char*)pw);
        printf("[DEBUG] Found user %s with ID %d and password %s\n", u->name, u->id, u->password); // Added debug
        ret = 1;
    } else {
        printf("[DEBUG] User %s not found in database.\n", u->name); // Added debug
    }

    sqlite3_finalize(stmt);
    return ret;
}


/**
 * Insert a new account record. 
 */
 int sql_create_account(struct User *u, struct Record r) {
    char dateStr[20];
    sprintf(dateStr, "%02d/%02d/%04d", 
        r.deposit.month, r.deposit.day, r.deposit.year);

    char query[256];
    sprintf(query,
        "INSERT INTO Accounts(user_id, date, country, phone, balance, type, account_nbr) "
        "VALUES(%d, '%s', '%s', %d, %.2f, '%s', %d);",
        u->id,
        dateStr,
        r.country,
        r.phone,
        r.amount,
        r.accountType,
        r.accountNbr
    );

    char *errMsg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("Error creating account: %s\n", errMsg);
        sqlite3_free(errMsg);
        return 0;
    }
    return 1;
}

int sql_select_accounts_for_user(struct User *u) {
    char query[256];
    sprintf(query, "SELECT account_id, date, country, phone, balance, type, account_nbr FROM Accounts WHERE user_id=%d;", u->id);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL prepare error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
        int acc_id          = sqlite3_column_int(stmt, 0);
        const char *date    = (const char*)sqlite3_column_text(stmt, 1);
        const char *country = (const char*)sqlite3_column_text(stmt, 2);
        int phone           = sqlite3_column_int(stmt, 3);
        double balance      = sqlite3_column_double(stmt, 4);
        const char *type    = (const char*)sqlite3_column_text(stmt, 5);
        int accNbr          = sqlite3_column_int(stmt, 6);

        printf("Account ID: %d\n", acc_id);
        printf("Account Nbr: %d\n", accNbr);
        printf("Date: %s\n", date);
        printf("Country: %s\n", country);
        printf("Phone: %d\n", phone);
        printf("Balance: %.2f\n", balance);
        printf("Type: %s\n\n", type);
    }
    sqlite3_finalize(stmt);
    return count;
}
