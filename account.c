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
#include "account.h"
#include "file.h" // for load_accounts_from_file()

// Placeholder function definitions
void create_account(Bank *bank)
{
    while (1)
    {
        int choice;

        printf("\n--- Create New Account ---\n");

        if (bank->account_count >= MAX_ACCOUNTS)
        {
            printf("\nBank account limit reached!\n");
            return;
        }

        printf("1. Create Savings Account\n");
        printf("2. Create Zero Balance Account\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ; // Clear invalid input
            printf("Invalid input! Please enter a valid number\n");
            continue;
        }

        Account *new_acc = &bank->accounts[bank->account_count];

        // Generate account number from file
        generate_account_number(new_acc->acc_no);

        printf("Generated Account Number: %s\n", new_acc->acc_no);

        while (1)
        {
            printf("Enter account holder name: ");
            scanf(" %[^\n]", new_acc->name);

            if (!is_valid_name(new_acc->name))
            {
                printf("❌ Invalid name. Only alphabets, spaces are allowed\n");
                continue; // ask again
            }

            break; // valid name entered
        }
        while (1)
        {
            printf("Enter 10-digit mobile number: ");
            scanf(" %[^\n]", new_acc->mobile);

            if (!is_validnumber(new_acc->mobile))
            {
                printf("❌ Invalid mobile number. Please enter a 10-digit number.\n");
                continue;
            }

            if (is_duplicate_number(bank, new_acc->mobile))
            {
                printf("⚠️  Mobile number already in use! Please enter a different one.\n");
                continue;
            }

            break; // Valid and unique
        }
        switch (choice)
        {
        case 1:
        {
            strcpy(new_acc->account_type, "Savings");

            while (1)
            {
                printf("Enter initial deposit amount (minimum ₹1000): ");
                if (scanf("%lf", &new_acc->balance) != 1)
                {
                    printf("❌ Invalid input! Please enter a valid amount.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }

                if (new_acc->balance < 1000.0)
                {
                    printf("⚠️  Minimum deposit for Savings account is ₹1000. Please try again.\n");
                    continue;
                }

                // Save changes
                update_account_balance(new_acc->acc_no, new_acc->balance);
                break;
            }
            break;
        }
        case 2:
        {
            strcpy(new_acc->account_type, "Zero Balance");
            new_acc->balance = 0.0;

            char choice;
            while (1)
            {
                printf("Do you want to deposit some amount now? (y/n): ");
                scanf(" %c", &choice);

                if (choice == 'y' || choice == 'Y')
                {
                    while (1)
                    {
                        printf("Enter amount to deposit: ₹");
                        if (scanf("%lf", &new_acc->balance) != 1)
                        {
                            printf("❌ Invalid input! Please enter a valid amount.\n");
                            while (getchar() != '\n')
                                ; // Clear input buffer
                            continue;
                        }

                        if (new_acc->balance <= 0)
                        {
                            printf("⚠️ Amount must be greater than ₹0. Please try again.\n");
                            continue;
                        }

                        break; // Valid amount
                    }
                    break; // Done with deposit
                }
                else if (choice == 'n' || choice == 'N')
                {
                    new_acc->balance = 0.0;
                    break; // Skip deposit
                }
                else
                {
                    printf("❌ Invalid input! Please enter 'y' or 'n'.\n");
                    while (getchar() != '\n')
                        ; // Clear input buffer
                }
            }

            update_account_balance(new_acc->acc_no, new_acc->balance);
            break;
        }
        case 3:
        {
            printf("Returning to main menu...\n");
            return;
        }
        default:
        {
            printf("\nInvalid choice! Please select 1, 2, or 0.\n");
            continue;
        }
        }

        strcpy(new_acc->status, "Active");
        get_current_date(new_acc->created_on, sizeof(new_acc->created_on));
        // strcpy(new_acc->created_on, "2025-06-24"); // Replace with dynamic date if needed

        // save to file
        save_accounts_to_file(*new_acc);
        bank->account_count++;

        printf("\nAccount created successfully!\n");
        // Ask if user wants to view updated details
        print_updated_details(new_acc);

        return; // Exit after successful creation
    }
}

void deposit_money(Bank *bank)
{
    char last4[5];
    double amount;
    int i, found = 0;

    printf("\n------ Deposit Money ------\n");

    while (1)
    {
        printf("Enter last 4 digits of account number: ");
        scanf(" %[^\n]", last4);

        // Clear input buffer
        while (getchar() != '\n')
            ;

        if (!is_validaccno(last4) || strlen(last4) != 4)
        {
            printf("❌ Invalid input. Please enter exactly 4 digits.\n");
            continue;
        }

        found = 0;

        for (i = 0; i < bank->account_count; i++)
        {
            int len = strlen(bank->accounts[i].acc_no);

            if (len >= 4 && strcmp(&bank->accounts[i].acc_no[len - 4], last4) == 0)
            {
                found = 1;
                printf("✅ Account found matching the last 4 digits: %s\n", last4);
                print_account_details(&bank->accounts[i]);

                while (1)
                {
                    printf("Enter amount to deposit: ₹");
                    if (scanf("%lf", &amount) != 1)
                    {
                        printf("❌ Invalid input. Please enter a numeric amount.\n");
                        while (getchar() != '\n')
                            ; // Clear input
                        continue;
                    }

                    if (amount <= 0)
                    {
                        printf("❌ Deposit amount must be greater than 0.\n");
                        continue;
                    }

                    if (bank->accounts[i].balance + amount > 10000000.0)
                    {
                        printf("❌ Deposit would exceed account balance limit (₹1 crore). Transaction cancelled.\n");
                        continue;
                    }

                    // Perform deposit
                    bank->accounts[i].balance += amount;
                    printf("✅ ₹%.2f deposited successfully.\n", amount);
                    printf("New Balance: ₹%.2f\n", bank->accounts[i].balance);

                    // Save changes
                    update_account_balance(bank->accounts[i].acc_no, bank->accounts[i].balance);

                    // Ask if user wants to view updated details
                    print_updated_details(&bank->accounts[i]);

                    // Ask the user for receipt
                    print_deposit_receipt(&bank->accounts[i], amount);

                    break; // Exit deposit loop
                }
                break; // Exit search loop
            }
        }

        if (!found)
        {
            printf("\n❌ No account found with last 4 digits: %s\n", last4);
            printf("Please try again.\n\n");
            continue;
        }

        break; // Exit main loop after successful deposit
    }
}

void withdraw_money(Bank *bank)
{
    char last4[5];
    int i, len, found;
    double amount;

    printf("\n------ Withdraw Money ------\n");

    while (1)
    {
        printf("Enter last 4 digits of account number: ");
        scanf(" %[^\n]", last4);
        while (getchar() != '\n')
            ;
        if (strlen(last4) != 4 || !is_validaccno(last4))
        {
            printf("❌ Invalid input. Please enter exactly 4 digits (numbers only).\n");
            continue;
        }

        found = 0;

        // Search for matching account
        for (i = 0; i < bank->account_count; i++)
        {
            len = strlen(bank->accounts[i].acc_no);

            if (len >= 4 && strcmp(&bank->accounts[i].acc_no[len - 4], last4) == 0)
            {
                found = 1;
                printf("✅ Account found matching the last 4 digits: %s\n", last4);
                print_account_details(&bank->accounts[i]);

                if (bank->accounts[i].balance == 0)
                {
                    printf("\n⚠️  Withdrawal not possible. Your balance is ₹0.00.\n");
                    return;
                }

                // Withdraw loop
                while (1)
                {
                    printf("Enter amount to withdraw: ₹");
                    if (scanf("%lf", &amount) != 1)
                    {
                        printf("❌ Invalid input! Please enter a valid amount.\n");
                        while (getchar() != '\n')
                            ; // Clear input buffer
                        continue;
                    }

                    if (amount <= 0)
                    {
                        printf("❌ Amount must be greater than 0\n");
                        continue;
                    }

                    // Check for savings minimum balance rule
                    if (strcmp(bank->accounts[i].account_type, "Savings") == 0 &&
                        (bank->accounts[i].balance - amount < 1000))
                    {
                        printf("\n⚠️ Cannot withdraw. Minimum ₹1000 must be maintained in Savings account.\n");
                        continue;
                    }

                    if (amount > bank->accounts[i].balance)
                    {
                        printf("❌ Insufficient balance. Your balance is ₹%.2f\n", bank->accounts[i].balance);
                        continue;
                    }

                    // Proceed with withdrawal
                    bank->accounts[i].balance -= amount;
                    printf("✅ ₹%.2f withdrawn successfully.\n", amount);
                    printf("New Balance: ₹%.2f\n", bank->accounts[i].balance);

                    // Save to file/database
                    update_account_balance(bank->accounts[i].acc_no, bank->accounts[i].balance);

                    // Ask user to view updated details
                    print_updated_details(&bank->accounts[i]);

                    break; // Exit withdrawal loop
                }

                break; // Exit outer loop after handling valid account
            }
        }

        if (!found)
        {
            printf("❌ No account found with last 4 digits '%s'. Try again.\n", last4);
            continue;
        }

        break; // If account was found and handled, exit loop
    }
}

void transfer_money(Bank *bank)
{
    char from_last4[5], to_last4[5];
    double amount;
    int from_index = -1, to_index = -1;

    printf("\n------ 💸 Money Transfer ------\n");

    // Get sender's account
    while (1)
    {
        printf("Enter last 4 digits of sender's account number: ");
        scanf(" %[^\n]", from_last4);
        while (getchar() != '\n')
            ;

        if (strlen(from_last4) != 4 || !is_validaccno(from_last4))
        {
            printf("❌ Invalid input. Please enter exactly 4 digits (numbers only).\n");
            continue;
        }

        for (int i = 0; i < bank->account_count; i++)
        {
            int len = strlen(bank->accounts[i].acc_no);
            if (len >= 4 && strcmp(&bank->accounts[i].acc_no[len - 4], from_last4) == 0)
            {
                from_index = i;
                break;
            }
        }

        if (from_index == -1)
        {
            printf("❌ Sender account not found. Try again.\n");
            continue;
        }
        else
        {
            print_account_details(&bank->accounts[from_index]);
            break;
        }
    }

    // Get receiver's account
    while (1)
    {
        printf("Enter last 4 digits of receiver's account number: ");
        scanf(" %[^\n]", to_last4);
        while (getchar() != '\n')
            ;

        if (strlen(to_last4) != 4 || !is_validaccno(to_last4))
        {
            printf("❌ Invalid input. Please enter exactly 4 digits (numbers only).\n");
            continue;
        }

        for (int i = 0; i < bank->account_count; i++)
        {
            int len = strlen(bank->accounts[i].acc_no);
            if (len >= 4 && strcmp(&bank->accounts[i].acc_no[len - 4], to_last4) == 0)
            {
                to_index = i;
                break;
            }
        }

        if (to_index == -1)
        {
            printf("❌ Receiver account not found. Try again\n");
            continue;
        }
        else if (to_index == from_index)
        {
            printf("❌ Sender and receiver accounts cannot be the same\n");
            continue;
        }
        else
        {
            print_account_details(&bank->accounts[to_index]);
            // Block transfer if balance is ₹0
            if (bank->accounts[from_index].balance == 0)
            {
                printf("❌ Transfers not allowed from an account with ₹0 balance.\n");
                return;
            }

            break;
        }
    }

    // Enter transfer amount
    while (1)
    {
        printf("Enter amount to transfer: ₹");
        if (scanf("%lf", &amount) != 1 || amount <= 0)
        {
            printf("❌ Invalid amount. Try again\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (strcmp(bank->accounts[from_index].account_type, "Savings") == 0 &&
            bank->accounts[from_index].balance - amount < 1000)
        {
            printf("❌ Savings account must maintain a minimum balance of ₹1000\n");
            continue;
        }
        else if (bank->accounts[from_index].balance < amount)
        {
            printf("❌ Not enough balance. Please enter a smaller amount\n");
            continue;
        }

        break;
    }

    // Perform transfer
    bank->accounts[from_index].balance -= amount;
    bank->accounts[to_index].balance += amount;

    printf("✅ ₹%.2lf transferred successfully from %s to %s!\n",
           amount, bank->accounts[from_index].name, bank->accounts[to_index].name);

    // Save updated accounts to file
    update_account_balance(bank->accounts[from_index].acc_no, bank->accounts[from_index].balance);
    update_account_balance(bank->accounts[to_index].acc_no, bank->accounts[to_index].balance);

    // Ask to print receipt
    print_receipt(&bank->accounts[from_index], &bank->accounts[to_index], amount);

    // Ask to show updated sender details
    print_updated_details(&bank->accounts[from_index]);
}

void display_balance(const Bank *bank)
{
    char last4[5];

    printf("\n------ Check Account Balance ------\n");

    while (1)
    {
        printf("Enter last 4 digits of your account number: ");
        scanf(" %[^\n]", last4);

        // Clear input buffer
        while (getchar() != '\n')
            ;

        if (strlen(last4) != 4 || !is_validaccno(last4))
        {
            printf("❌ Invalid input. Please enter exactly 4 digits (numbers only).\n");
            continue;
        }

        for (int i = 0; i < bank->account_count; i++)
        {
            int len = strlen(bank->accounts[i].acc_no);
            if (len >= 4 && strcmp(&bank->accounts[i].acc_no[len - 4], last4) == 0)
            {
                if (strcmp(bank->accounts[i].status, "Closed") == 0)
                {
                    printf("⚠️ This account is closed. Balance cannot be displayed.\n");
                    return;
                }

                printf("✅ Account found!\n");
                print_account_details(&bank->accounts[i]);
                return;
            }
        }

        printf("❌ No account found with last 4 digits '%s'. Please try again.\n\n", last4);
    }
}

void list_all_accounts(const Bank *bank)
{
    if (bank->account_count == 0)
    {
        printf("\nNo accounts to display.\n");
        return;
    }

    printf("\n-------- There are %d accounts --------\n", bank->account_count);
    printf("\n----------- Account List ----------\n");

    for (int i = 0; i < bank->account_count; i++)
    {
        const Account *acc = &bank->accounts[i]; // ✅ fixed: now const

        printf("\n------------ Account %d ------------\n", i + 1);
        printf("  %-17s : %s\n", "Account Number", acc->acc_no);
        printf("  %-17s : %s\n", "Name", acc->name);
        printf("  %-17s : %s\n", "Mobile", acc->mobile);
        printf("  %-17s : %s\n", "Account Type", acc->account_type);
        printf("  %-17s : %.2lf\n", "Balance", acc->balance);
        printf("  %-17s : %s\n", "Status", acc->status);
        printf("  %-17s : %s\n", "Created On", acc->created_on);
    }

    printf("\n------------------------------------\n");
}

void update_my_mobile(Bank *bank)
{
    char last4[5];              // 4 digits + null-terminator
    char acc_no[ACC_NO_LENGTH]; // To store the matched full account number
    int found = 0;

    while (1)
    {
        printf("Enter last 4 digits of account number: ");
        scanf(" %[^\n]", last4);

        // Clear input buffer
        while (getchar() != '\n')
            ;

        if (!is_validaccno(last4) || strlen(last4) != 4)
        {
            printf("❌ Invalid input. Please enter exactly 4 digits.\n");
            continue;
        }

        for (int i = 0; i < bank->account_count; i++)
        {
            const char *full_acc = bank->accounts[i].acc_no;
            if (strlen(full_acc) >= 4 &&
                strcmp(&full_acc[strlen(full_acc) - 4], last4) == 0)
            {
                strcpy(acc_no, full_acc); // ✅ Store full account number
                print_account_details(&bank->accounts[i]);
                found = 1;
                break; // Stop at first match
            }
        }

        if (!found)
        {
            printf("⚠️  No account found with those 4 digits\n");
            continue; // if not found
        }

        break; // Exit loop if valid account found
    }

    // Now call update with valid full acc_no
    update_mobile_number(acc_no, bank);
}
