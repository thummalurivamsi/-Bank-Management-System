/*
Documentation
Name        : Vamsi T
Date        : 25/6/25
Description : Bank Management System Project
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "account.h"
#include "file.h" // for load_accounts_from_file()

int is_valid_name(const char *name)
{
    int len = strlen(name);

    if (len < 2 || len > 50)
        return 0; // Too short or too long

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0; // Only allow alphabets, space
    }

    return 1;
}
int is_validnumber(const char *number)
{
    int i = 0;

    // Check if the number has exactly 10 digits
    while (number[i])
    {
        if (number[i] < '0' || number[i] > '9') // Check if each char is digit
            return 0;
        i++;
    }

    if (i != 10) // Not 10 digits
        return 0;

    return 1;
}
// Duplicate funtion
int is_duplicate_number(const Bank *bank, const char *mobile)
{
    for (int i = 0; i < bank->account_count; i++)
    {
        if (strcmp(bank->accounts[i].mobile, mobile) == 0)
        {
            return 1; // Duplicate found
        }
    }
    return 0; // Unique
}

int is_validaccno(const char *acc_no)
{
    int i = 0;

    while (acc_no[i])
    {
        // Invalid: non-digit character found
        if (acc_no[i] < '0' || acc_no[i] > '9')
            return 0;
        i++;
    }
    if (i != 4)
        return 0;
    return 1; // all characters are digits
}

void get_current_date(char *buffer, size_t size)
{
    time_t now = time(NULL);
    // strftime = string format time
    // Converts the struct tm date into a formatted string
    // Stores the result in buffer.
    strftime(buffer, size, "%Y-%m-%d", localtime(&now));
    // localtime(&now)
    // Converts time_t to a local time representation (your system's time zone)
}

void print_account_details(const Account *acc)
{
    // Display account details
    printf("\n--------- Account Details ----------\n");
    printf("  %-17s : %s\n", "Account Number", acc->acc_no);
    printf("  %-17s : %s\n", "Name", acc->name);
    printf("  %-17s : %s\n", "Mobile", acc->mobile); // Added line
    printf("  %-17s : %s\n", "Account Type", acc->account_type);
    printf("  %-17s : %s\n", "Status", acc->status);
    printf("  %-17s : %s\n", "Created On", acc->created_on);
    printf("  %-17s : %.2lf\n", "Balance", acc->balance);
}

void print_updated_details(Account *account)
{
    char choice;

    while (1)
    {
        printf("\nDo you want to see the updated account details? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            print_account_details(account);
            break;
        }
        else if (choice == 'n' || choice == 'N')
        {
            printf("Okay, returning to main menu\n");
            break;
        }
        else
        {
            printf("\n❌ Invalid input. Please enter 'y' or 'n'\n");
        }
    }
}
void generate_transaction_id(char *txn_id)
{
    time_t now = time(NULL); // Retrieves the current system time as the number of
                             // seconds since the Unix epoch (1970-01-01 00:00:00)
    struct tm *t = localtime(&now);

    // Format: TXNYYYYMMDDHHMMSS
    sprintf(txn_id, "TXN%04d%02d%02d%02d%02d%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    /*
    t->tm_year → Years since 1900
    t->tm_mon → Months since January (0–11)
    t->tm_mday → Day of the month (1–31)
    t->tm_hour → Hour (0–23)
    t->tm_min → Minute (0–59)
    t->tm_sec → Second (0–59)
    */
}
// Function to print transfer receipt if user agrees
// Helper to mask account number
void mask_account_number(const char *acc_no, char *masked)
{
    int len = strlen(acc_no);
    if (len <= 4)
        strcpy(masked, acc_no);
    else
        sprintf(masked, "******%s", &acc_no[len - 4]);
}

void print_receipt(Account *sender, Account *receiver, double amount)
{
    char choice;
    char txn_id[25];
    generate_transaction_id(txn_id);
    // existing date/time
    time_t now = time(NULL);
    char *dt = ctime(&now);
    dt[strcspn(dt, "\n")] = '\0'; // remove newline

    while (1)
    {
        printf("\nDo you want to print the transaction receipt? (y/n): ");
        scanf(" %c", &choice);
        while (getchar() != '\n')
            ; // clear buffer

        if (choice == 'y' || choice == 'Y')
        {
            // Generate masked account numbers
            char masked_sender[20], masked_receiver[20];
            mask_account_number(sender->acc_no, masked_sender);
            mask_account_number(receiver->acc_no, masked_receiver);

            // Get current date and time
            time_t now = time(NULL);
            char *dt = ctime(&now); // ctime() converts time_t to a human-readable string format
            // strcspn(dt, "\n") finds the position of the newline character (\n) in the string
            dt[strcspn(dt, "\n")] = '\0'; // remove newline
            // dt[...] = '\0'; replaces the newline with the null terminator \0

            printf("\n========= 🧾 Transaction Receipt =========\n");
            printf("Transaction ID: %s\n", txn_id);
            printf("From   : %s (Acc No: %s)\n", sender->name, masked_sender);
            printf("To     : %s (Acc No: %s)\n", receiver->name, masked_receiver);
            printf("Amount : ₹%.2lf\n", amount);
            printf("Remaining Balance in Sender's Account: ₹%.2lf\n", sender->balance);
            printf("Date & Time: %s\n", dt);
            printf("==========================================\n");
            break;
        }
        else if (choice == 'n' || choice == 'N')
        {
            printf("✅ Transaction completed. Receipt not printed.\n");
            break;
        }
        else
        {
            printf("❌ Invalid input. Please enter 'y' or 'n'.\n");
        }
    }
}

void print_deposit_receipt(Account *account, double amount)
{
    char choice;
    char txn_id[25];
    generate_transaction_id(txn_id);
    // existing date/time
    time_t now = time(NULL);
    char *dt = ctime(&now);
    dt[strcspn(dt, "\n")] = '\0'; // remove newline

    while (1)
    {
        printf("\nDo you want to print the deposit receipt? (y/n): ");
        scanf(" %c", &choice);
        while (getchar() != '\n')
            ;

        if (choice == 'y' || choice == 'Y')
        {
            char masked_acc[20];
            mask_account_number(account->acc_no, masked_acc);

            time_t now = time(NULL);
            char *dt = ctime(&now); // ctime() converts time_t to a human-readable string format
                                    // strcspn(dt, "\n") finds the position of the newline character (\n) in the string
            dt[strcspn(dt, "\n")] = '\0';
            // dt[...] = '\0'; replaces the newline with the null terminator \0

            printf("\n========= 🧾 Deposit Receipt =========\n");
            printf("Transaction ID: %s\n", txn_id);
            printf("Account Holder : %s\n", account->name);
            printf("Mobile Number  : %s\n", account->mobile);
            printf("Account Number : %s\n", masked_acc);
            printf("Deposited      : ₹%.2lf\n", amount);
            printf("New Balance    : ₹%.2lf\n", account->balance);
            printf("Date & Time    : %s\n", dt);
            printf("======================================\n");
            break;
        }
        else if (choice == 'n' || choice == 'N')
        {
            printf("✅ Deposit completed. Receipt not printed.\n");
            break;
        }
        else
        {
            printf("❌ Invalid input. Please enter 'y' or 'n'.\n");
        }
    }
}
