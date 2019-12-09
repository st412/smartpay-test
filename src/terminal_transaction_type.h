#ifndef _TERMINAL_TRANSACTION_TYPE_H
#define _TERMINAL_TRANSACTION_TYPE_H

#include <stddef.h>

enum TransactionType {
    CHEQUE,
    SAVINGS,
    CREDIT
};

char* TransactionType_to_string(int transaction_type);
int TransactionType_to_int(char* transaction_type);
int TransactionType_is_valid(char* transaction_type);

#endif
