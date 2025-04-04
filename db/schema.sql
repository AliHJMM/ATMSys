DROP TABLE IF EXISTS Transactions;
DROP TABLE IF EXISTS Accounts_Transfers;
DROP TABLE IF EXISTS Accounts;
DROP TABLE IF EXISTS Users;

-- Users table
CREATE TABLE Users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL
);

-- Accounts table
CREATE TABLE Accounts (
    account_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    type TEXT NOT NULL CHECK (
        type IN ('fixed01', 'fixed02', 'fixed03', 'current', 'saving')
    ),
    date TEXT NOT NULL, -- stored as 'DD/MM/YYYY'
    balance REAL NOT NULL,
    country TEXT NOT NULL,
    phone INTEGER NOT NULL UNIQUE,
    account_nbr INTEGER NOT NULL UNIQUE,
    FOREIGN KEY (user_id) REFERENCES Users(id)
);


-- Transfers table (optional, for future use)
CREATE TABLE Accounts_Transfers (
    transfer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    receiver_user_id INTEGER NOT NULL,
    transfered_account_id INTEGER NOT NULL,
    date TEXT NOT NULL,
    FOREIGN KEY (receiver_user_id) REFERENCES Users(id),
    FOREIGN KEY (transfered_account_id) REFERENCES Accounts(account_id)
);

-- Transactions table (optional, for future use)
CREATE TABLE Transactions (
    transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,
    sender_account_id INTEGER NOT NULL,
    receiver_account_id INTEGER NOT NULL,
    amount REAL NOT NULL,
    date TEXT NOT NULL,
    FOREIGN KEY (sender_account_id) REFERENCES Accounts(account_id),
    FOREIGN KEY (receiver_account_id) REFERENCES Accounts(account_id)
);
