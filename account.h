#ifndef ACCOUNT_H
#define ACCOUNT_H

#define MAX_ACCOUNTS 100
#define ACC_NO_LENGTH 12 // 11 digits + '\0'
#define NAME_LENGTH 50
#define MOBILE_LENGTH 15
#define TYPE_LENGTH 20
#define STATUS_LENGTH 10
#define DATE_LENGTH 11 // Format: YYYY-MM-DD
#define ACCOUNT_FILE "accounts.txt"

// Account structure with string account number
typedef struct
{
    char acc_no[ACC_NO_LENGTH]; //  "0000123456"
    char name[NAME_LENGTH];
    char mobile[MOBILE_LENGTH];
    char account_type[TYPE_LENGTH]; // "Savings", "Zero Balance"
    char status[STATUS_LENGTH];     // "Active", "Closed"
    char created_on[DATE_LENGTH];   // "2025-06-24"
    double balance;
} Account;

// Bank structure
typedef struct
{
    Account accounts[MAX_ACCOUNTS];
    int account_count;
} Bank;

// Function declarations

// Bank Initialization & Account Management
void create_account(Bank *bank);

// Banking Transactions
void deposit_money(Bank *bank);
void withdraw_money(Bank *bank);
void transfer_money(Bank *bank);

// Account Display & Listing
void display_account(const Bank *bank);
void display_balance(const Bank *bank);
void list_all_accounts(const Bank *bank);

// Validation Utilities
int is_valid_name(const char *name);
int is_validnumber(const char *mobile);
int is_validaccno(const char *acc_no);
int is_duplicate_number(const Bank *bank, const char *mobile);

// Account Number & Date Utilities
void generate_account_number(char *acc_no);
void get_current_date(char *buffer, size_t size);
void update_my_mobile(Bank *bank);

// Receipt & Transaction Utilities
void print_account_details(const Account *acc);
void print_updated_details(Account *account);
void print_receipt(Account *sender, Account *receiver, double amount);
void mask_account_number(const char *acc_no, char *masked);
void print_deposit_receipt(Account *account, double amount);
void generate_transaction_id(char *txn_id);

#endif // ACCOUNT_H
