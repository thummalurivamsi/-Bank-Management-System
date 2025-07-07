📒 Bank Management System in C

This project is a terminal-based Bank Management System developed in the C programming language. It simulates basic banking operations such as creating accounts, deposits, withdrawals, money transfers, balance inquiries, and mobile number updates. Built using modular programming practices in C, the system includes robust file handling, input validation, and error checking to ensure data integrity and reliability in financial transaction simulations.

📌 Project Overview

Manual banking methods are time-consuming and error-prone. This project introduces a digital, command-line solution that allows users to perform banking operations efficiently. It uses persistent storage with text files and follows a menu-driven approach for real-time user interaction. Data is stored and retrieved using file I/O, ensuring updates are safely written to disk.

⚙️ Components / Tools Used

C Programming Language

GCC Compiler

Command-line Interface (CLI)

Text File Storage (accounts.txt)

Modular File Structure

📁 File Modules

main.c – Entry point and main menu

account.c / account.h – Core banking logic

transaction.c / transaction.h – Deposit, withdraw, transfer

file.c / file.h – Input validation and duplicate checking

accounts.txt – File-based persistent storage of account data

🧠 Working Principle

The system uses a structured menu interface allowing the following operations:

🔹 Create Account
Generates a unique account number and accepts account holder details. Validates and checks for duplicates (based on mobile number).

🔹 Deposit Money
Deposits an amount into a specified account after validation.

🔹 Withdraw Money
Withdraws funds from an account after balance checking to prevent overdrafts.

🔹 Money Transfer
Transfers funds between two valid accounts with error handling for balance and account existence.

🔹 Check Balance
Displays the current balance of a specified account.

🔹 List All Accounts
Displays details of all accounts in a formatted table view.

🔹 Update Mobile Number
Allows the user to update the registered mobile number with input validation.

🔹 Exit
Safely writes all modifications and exits.

🔐 Input Validation Rules

Mobile Number: Must be exactly 10 digits, numeric only
Example: 9876543210

Name: Alphabets only, no special characters or digits
Example: Anju Thummaluri

Amount: Must be a positive numeric value

Duplicate mobile numbers are rejected. All inputs are verified before being saved.

💡 Features

✅ Add, update, list, and remove accounts
✅ Deposit, withdraw, and transfer funds
✅ Unique account number generator
✅ Mobile number update with validation
✅ Persistent file storage
✅ Modular and maintainable code
✅ Supports multiple accounts and transactions
✅ Duplicate prevention logic for mobile numbers

📂 File Structure

less
Copy
Edit
├── main.c              // Main menu logic
├── account.c/.h        // Account operations (create, update, list)
├── transaction.c/.h    // Deposit, withdraw, transfer functions
├── definition.c/.h          // Validation utilities
├── accounts.txt        // Account data storage
├── README.md           // Project documentation
📸 Project Media

📽️ Demo Video: Watch here 
https://drive.google.com/file/d/1_eT1m3UE734FzPALXcElZKrH_o8I9Slg/view?usp=sharing
🖼️ Sample Terminal Output:
![fig-1](https://github.com/user-attachments/assets/829c369b-fa1c-4d74-a51d-9964c6f5a19e)

![fig-2](https://github.com/user-attachments/assets/22788caf-0bc6-4756-b09f-1a1994e84cc7)

📈 Future Improvements

GUI version using GTK or Qt

Integration with SQLite for scalable database support

Transaction history and printable receipts

User authentication with password protection

Role-based access (admin vs user)

OTP or 2FA for secure transactions

👨‍💻 Developed By
Vamsi T
Graduate Engineer – Embedded Systems Enthusiast
📧 vamsithummaluri@example.com

