#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>

void Terminal_destroy(struct Terminal* t)
{
    if (t)
    {
        if (t->card_types)
        {
            free(t->card_types);
        }
        if (t->transaction_types)
        {
            free(t->transaction_types);
        }
        free(t);
    }
}
