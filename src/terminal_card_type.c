#include "terminal_card_type.h"

#include <string.h>

#define VISA_STR "Visa"
#define MASTERCARD_STR "MasterCard"
#define EFTPOS_STR "EFTPOS"

char* CardType_to_string(int card_type)
{
    if (card_type == VISA)
    {
        return VISA_STR;
    }
    else if (card_type == MASTERCARD)
    {
        return MASTERCARD_STR;
    }
    else if (card_type == EFTPOS)
    {
        return EFTPOS_STR;
    }
    else
    {
        return "Unknown";
    }
}

int CardType_to_int(char* card_type)
{
    if (strcmp(card_type, VISA_STR) == 0)
    {
        return VISA;
    }
    else if (strcmp(card_type, MASTERCARD_STR) == 0)
    {
        return MASTERCARD;
    }
    else if (strcmp(card_type, EFTPOS_STR) == 0)
    {
        return EFTPOS;
    }
    else
    {
        return -1;
    }
}

int CardType_is_valid(char* card_type)
{
    if(strcmp(card_type, VISA_STR) == 0 ||
       strcmp(card_type, MASTERCARD_STR) == 0 ||
       strcmp(card_type, EFTPOS_STR) == 0)
    {
        return 0;
    }
    return -1;
}
