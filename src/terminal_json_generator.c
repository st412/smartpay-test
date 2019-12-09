#include "terminal_json_generator.h"

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>

#include "terminal_json_constants.h"
#include "terminal_card_type.h"
#include "terminal_transaction_type.h"

#define TERMINAL_GEN_BASE_BUFFER_SZ 1024
#define MAX_ID_DIGITS 10

static void append_json(char* dest, char* src, size_t* dest_sz);
static void append_terminal_json(struct Terminal* t, char* buffer, size_t* buffer_sz);

char* TerminalJsonGenerator_generate(struct Terminal** ts, size_t ts_sz)
{
    if (ts == NULL)
    {
        fprintf(stderr, "[TerminalJsonGenerator_generate] "
            "Failed to generate JSON, param \"ts\" is NULL.\n");
        return NULL;
    }

    size_t buffer_sz = TERMINAL_GEN_BASE_BUFFER_SZ;
    char* buffer = malloc(buffer_sz);
    memset(buffer, 0, buffer_sz);

    append_json(buffer, "[", &buffer_sz);
    for (size_t i = 0; i < ts_sz; ++i)
    {
        append_terminal_json(ts[i], buffer, &buffer_sz);
        if (i + 1 < ts_sz)
        {
            append_json(buffer, ",", &buffer_sz);
        }
    }
    append_json(buffer, "]", &buffer_sz);

    return buffer;
}

char* TerminalJsonGenerator_generate_one(struct Terminal* t)
{
    size_t buffer_sz = TERMINAL_GEN_BASE_BUFFER_SZ;
    char* buffer = malloc(buffer_sz);
    memset(buffer, 0, buffer_sz);
    append_terminal_json(t, buffer, &buffer_sz);
    return buffer;
}

static void append_json(char* dest, char* src, size_t* dest_sz)
{
    if (strlen(src) + strlen(dest) > *dest_sz)
    {
        *dest_sz = (strlen(src) + strlen(dest)) * 2;
        dest = realloc(dest, *dest_sz);   
    }
    strcat(dest, src);
}

static void append_terminal_json(struct Terminal* t, char* buffer, size_t* buffer_sz)
{
    // Start token
    append_json(buffer, "{", buffer_sz);

    // Id
    char id_string[MAX_ID_DIGITS] = { 0 };
    sprintf(id_string, "\"%d\",", t->id);
    append_json(buffer, "\"id\": ", buffer_sz);
    append_json(buffer, id_string, buffer_sz);
    
    // Card types
    append_json(buffer, "\"" JSON_CARD_TYPE_NAME "\": [", buffer_sz);
    for (size_t ct = 0; ct < t->card_types_sz; ++ct)
    {
        char* card_type = CardType_to_string(t->card_types[ct]);
        append_json(buffer, "\"", buffer_sz);
        append_json(buffer, card_type, buffer_sz);
        append_json(buffer, "\"", buffer_sz);

        if (ct + 1 < t->card_types_sz)
        {
            append_json(buffer, ",", buffer_sz);
        }
    }
    append_json(buffer, "],", buffer_sz);

    // Transaction types
    append_json(buffer, "\"" JSON_TRANSACTION_TYPE_NAME "\": [", buffer_sz);
    for (size_t tt = 0; tt < t->transaction_types_sz; ++tt)
    {
        char* transaction_type = TransactionType_to_string(t->transaction_types[tt]);
        append_json(buffer, "\"", buffer_sz);
        append_json(buffer, transaction_type, buffer_sz);
        append_json(buffer, "\"", buffer_sz);
        if (tt + 1 < t->transaction_types_sz)
        {
            append_json(buffer, ",", buffer_sz);
        }
    }
    append_json(buffer, "]", buffer_sz);

    // End token
    append_json(buffer, "}", buffer_sz);
}
