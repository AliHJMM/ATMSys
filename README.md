# ATM Management System

A terminal-based ATM management application written in **C**, using **SQLite** for data storage. This project demonstrates core programming logic, file/database handling, user authentication, and CRUD operations through a user-friendly terminal UI.

---

## 📌 Features

### ✅ User Authentication

- Register new users (unique usernames only)
- Login system with password validation
- Secure interaction with SQLite (no plain text file handling)

### 🏦 Account Management

- Create Account with the following fields:
  - Account number (unique, 8–12 digits)
  - Creation date
  - Country (letters only)
  - Phone number (unique, 8 digits)
  - Balance (up to $5000)
  - Account type (`saving`, `current`, `fixed01`, `fixed02`, `fixed03`)
- View all accounts owned by a user
- View specific account details with interest calculation:
  - `saving`: 7% annual (monthly)
  - `fixed01`: 4% (1 year)
  - `fixed02`: 5% (2 years)
  - `fixed03`: 8% (3 years)
  - `current`: No interest

### 🔄 Update Account

- Update account country or phone number
- Validates format and uniqueness where needed

### 💸 Transactions

- Deposit and withdraw money
- Allowed only for `saving` and `current` accounts
- Validates amount and balance

### 🗑️ Delete Account

- Delete accounts owned by logged-in user
- Validates account number and ownership

### 🔁 Transfer Ownership

- Transfer account to another registered user
- Instant update in database

---

## 🎁 Bonus Features

- SQLite relational database integration
- Strong input validation for all fields
- Descriptive error messages and retry loops
- Interest calculation logic with real-time date parsing
- Modular design with `auth.c`, `system.c`, `sql.c`, and `header.h`
- Future TUI (Terminal UI) enhancement with ANSI colors and formatting
- [Planned] Real-time notifications for ownership transfer via IPC

---

## 🛠️ Technical Stack

- **Language**: C (C99 standard)
- **Database**: SQLite3
- **Build Tool**: Makefile
- **UI**: Terminal (CLI)
- **Architecture**: Modular C files with separate concerns

---

## 📂 Project Structure

ATM-System/
├── data/
│ ├── users.txt # [Legacy] User data (unused with DB)
│ ├── records.txt # [Legacy] Account data (unused with DB)
├── db/
│ └── atm.db # SQLite database
├── src/
│ ├── auth.c # Login and registration logic
│ ├── system.c # Main business logic (account ops)
│ ├── sql.c # SQLite query handling
│ ├── header.h # Structs, prototypes, and constants
│ └── main.c # Entry point with menus
├── Makefile # Build instructions
└── README.md # Project documentation

## 🚀 Getting Started

### 📋 Prerequisites

- GCC Compiler (`gcc`)
- SQLite3 (`libsqlite3-dev` or similar)
- Linux/macOS terminal or WSL on Windows

### 🧰 Build & Run

1. Clone the repository:

```bash
git clone https://learn.reboot01.com/git/alihasan6/atm-management-system
cd atm-system
```

2. Compile the project:

```bash
make
```

2. Run the application:

```bash
./atm.exe
```

### 📖 Learning Outcomes

- Mastery of C programming and memory management
- Database integration using SQLite C API
- Clean and secure handling of user input and validation
- Modular programming with header files
- Real-world project experience in terminal application design

## **Authors**

- [Ali Hasan](https://github.com/AliHJMM)
