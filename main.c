/*
Documentation
Name        : Vamsi T
Date        : 25/6/25
Description : Bank Management System Project
*/

#include <stdio.h>
#include "account.h"
#include "file.h"

void print_menu();

int main()
{
    Bank bank;
    bank.account_count = 0;
    load_accounts_from_file(&bank);
    int choice;

    while (1)
    {
        print_menu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a valid number\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            create_account(&bank);
            break;
        case 2:
            deposit_money(&bank);
            break;
        case 3:
            withdraw_money(&bank);
            break;
        case 4:
            transfer_money(&bank);
            break;
        case 5:
            display_balance(&bank);
            break;
        case 6:
            list_all_accounts(&bank);
            break;
        case 7:
            update_my_mobile(&bank);
            break;
        case 8:
            printf("Exit...\n");
            return 0;
        default:
            printf("Invalid choice! Try again\n");
        }
    }
}

void print_menu()
{
    printf("\n*Bank Management Menu*\n");
    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Transfer Money\n");
    printf("5. Check Account Balance\n");
    printf("6. List All Accounts\n");
    printf("7. Upadate Mobile\n");
    printf("8. Exit\n");
}
