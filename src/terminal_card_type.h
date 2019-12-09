#ifndef _TERMINAL_CARD_TYPE_H
#define _TERMINAL_CARD_TYPE_H

#include <stddef.h>

enum CardType {
    VISA,
    MASTERCARD,
    EFTPOS
};

char* CardType_to_string(int card_type);
int CardType_to_int(char* card_type);
int CardType_is_valid(char* card_type);

#endif
