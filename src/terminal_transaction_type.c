#include "terminal_transaction_type.h"

#include <string.h>

#define CHEQUE_STR  "Cheque"
#define SAVINGS_STR "Savings"
#define CREDIT_STR  "Credit"

char* TransactionType_to_string(int transaction_type)
{
    if (transaction_type == CHEQUE)
    {
        return CHEQUE_STR;
    }
    else if (transaction_type == SAVINGS)
    {
        return SAVINGS_STR;
    }
    else if (transaction_type == CREDIT)
    {
        return CREDIT_STR;
    }
    else
    {
        return "Unknown";
    }
}

int TransactionType_to_int(char* transaction_type)
{
    if (strcmp(transaction_type, CHEQUE_STR) == 0)
    {
        return CHEQUE;
    }
    else if (strcmp(transaction_type, SAVINGS_STR) == 0)
    {
        return SAVINGS;
    }
    else if (strcmp(transaction_type, CREDIT_STR) == 0)
    {
        return CREDIT;
    }
    else
    {
        return -1;
    }
}

int TransactionType_is_valid(char* transaction_type)
{
    if(strcmp(transaction_type, CHEQUE_STR) == 0 ||
       strcmp(transaction_type, SAVINGS_STR) == 0 ||
       strcmp(transaction_type, CREDIT_STR) == 0)
    {
        return 0;
    }
    return -1;
}
