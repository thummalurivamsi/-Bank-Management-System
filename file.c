/*
Documentation
Name        : Vamsi T
Date        : 25/6/25
Description : Bank Management System Project
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "account.h"

void generate_account_number(char *acc_no)
{
    FILE *fp = fopen("account_no.txt", "r+");
    int last_no = 0;

    if (fp == NULL)
    {
        // Create the file and write initial value
        fp = fopen("account_no.txt", "w+");
        if (fp == NULL)
        {
            //	Prints a system error message
            perror("Failed to create account number file");
            // Terminates the program with a status code
            exit(1);
        }
        last_no = 10;
    }
    else
    {
        if (fscanf(fp, "%d", &last_no) != 1)
            last_no = 9; // so after ++ it becomes 10
    }

    last_no++;
    // moves the file pointer to the start of the file so we can overwrite it
    rewind(fp);
    fprintf(fp, "%d", last_no);
    // ensures the data is written immediately to disk (flushes output buffer)
    fflush(fp);
    fclose(fp);

    // Format as 11-digit account number string
    // snprintf() stores a formatted string into acc_no
    //%011d → Format specifier that means
    //%d → print as integer
    // 011 → pad with zeros to make the number 11 digits long
    // For example, if last_no = 12, the result is "00000000012"
    snprintf(acc_no, ACC_NO_LENGTH, "%011d", last_no);
}

void load_accounts_from_file(Bank *bank)
{
    FILE *fp = fopen("accounts.txt", "r");
    if (fp == NULL)
    {
        perror("Could not open accounts.txt for reading");
        return;
    }

    Account acc;
    bank->account_count = 0;

    while (fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%lf\n",
                  acc.acc_no, acc.name, acc.mobile, acc.account_type,
                  acc.status, acc.created_on, &acc.balance) == 7)
    {
        if (bank->account_count < MAX_ACCOUNTS)
        {
            bank->accounts[bank->account_count++] = acc;
        }
        else
        {
            printf("⚠️  Maximum account limit reached. Cannot load more.\n");
            break;
        }
    }

    fclose(fp);
    // printf("✅ %d account(s) loaded successfully.\n", bank->account_count);
}

void save_accounts_to_file(Account new_acc)
{
    FILE *fp = fopen("accounts.txt", "a");
    if (fp == NULL)
    {
        perror("Failed to open file to append new account");
        return;
    }

    fprintf(fp, "%s,%s,%s,%s,%s,%s,%.2lf\n",
            new_acc.acc_no,
            new_acc.name,
            new_acc.mobile,
            new_acc.account_type,
            new_acc.status,
            new_acc.created_on,
            new_acc.balance);

    fclose(fp);
}

void update_account_balance(const char *acc_no_to_update, double new_balance)
{
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL)
    {
        perror("File open error");
        return;
    }

    Account acc;
    char line[256];

    while (fgets(line, sizeof(line), fp))
    {
        // Match the structure's order: acc_no, name, mobile, account_type, status, created_on, balance
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%lf",
               acc.acc_no, acc.name, acc.mobile, acc.account_type,
               acc.status, acc.created_on, &acc.balance);

        if (strcmp(acc.acc_no, acc_no_to_update) == 0)
        {
            acc.balance = new_balance;
        }

        // Write back in the same order
        fprintf(temp, "%s,%s,%s,%s,%s,%s,%.2lf\n",
                acc.acc_no, acc.name, acc.mobile, acc.account_type,
                acc.status, acc.created_on, acc.balance);
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    if (new_balance > 0.0)
        printf("Balance updated successfully.\n");
}

void update_mobile_number(const char *acc_no_to_update, const Bank *bank)
{
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL)
    {
        perror("File open error");
        return;
    }

    Account acc;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp))
    {
        // FIXED FIELD ORDER
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%lf",
               acc.acc_no, acc.name, acc.mobile,
               acc.account_type, acc.status, acc.created_on, &acc.balance);

        if (strcmp(acc.acc_no, acc_no_to_update) == 0)
        {
            found = 1;

            char confirm_name[NAME_LENGTH];
            while (1)
            {
                printf("Enter account holder name for verification: ");
                scanf(" %[^\n]", confirm_name);
                while (getchar() != '\n')
                    ;

                if (strcmp(confirm_name, acc.name) != 0)
                    printf("❌ Name mismatch. Try again.\n");
                else
                    break;
            }

            char new_mobile[MOBILE_LENGTH];
            while (1)
            {
                printf("Enter 10-digit mobile number: ");
                scanf(" %[^\n]", new_mobile);
                while (getchar() != '\n')
                    ;

                if (!is_validnumber(new_mobile))
                {
                    printf("❌ Invalid mobile number. Please enter a 10-digit number.\n");
                    continue;
                }

                if (strcmp(acc.mobile, new_mobile) == 0)
                {
                    printf("⚠️  This is already your registered number.\n");
                    break; // exit without change
                }

                if (is_duplicate_number((Bank *)bank, new_mobile))
                {
                    printf("⚠️  Mobile number already in use! Please enter a different one.\n");
                    continue;
                }

                strcpy(acc.mobile, new_mobile);
                printf("✅ Mobile number updated successfully.\n");
                break;
            }
        }

        // FIXED FIELD ORDER
        fprintf(temp, "%s,%s,%s,%s,%s,%s,%.2lf\n",
                acc.acc_no, acc.name, acc.mobile,
                acc.account_type, acc.status, acc.created_on, acc.balance);
    }

    fclose(fp);
    fclose(temp);
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    if (!found)
    {
        printf("⚠️  Account number not found.\n");
    }
}
