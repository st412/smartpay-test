#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stddef.h>

struct Terminal {
    int id;

    int* card_types;
    size_t card_types_sz;

    int* transaction_types;
    size_t transaction_types_sz;
};

void Terminal_destroy(struct Terminal* t);

#endif
