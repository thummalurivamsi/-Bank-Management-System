#ifndef FILE_H
#define FILE_H

#include "account.h" // Ensure Bank and Account structures are available

void save_accounts_to_file(Account new_acc);
void load_accounts_from_file(Bank *bank);
void update_account_balance(const char *acc_no_to_update, double new_balance);
void generate_account_number(char *acc_no);
void update_mobile_number(const char *acc_no_to_update, const Bank *bank);

#endif
