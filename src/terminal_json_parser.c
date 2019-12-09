#include "terminal_json_parser.h"
#include "json.h"

#include <stdio.h>
#include <string.h>

#include "terminal_card_type.h"
#include "terminal_transaction_type.h"
#include "terminal_json_constants.h"

static int parse_card_types(struct Terminal* t, json_value* card_types_json);
static int parse_transaction_types(struct Terminal* t, json_value* transaction_types_json);

struct Terminal* TerminalJsonParser_ParseTerminal(const char* json)
{
    json_value* t_json_val = json_parse(json, strlen(json));
    if (t_json_val == NULL)
    {
        fprintf(stderr, "[TerminalJsonParser_ParseTerminal]"
            " Failed to parse JSON: \n%s\n", json);
        return NULL;
    }

    if (t_json_val->type != json_object)
    {
        fprintf(stderr, "[TerminalJsonParser_ParseTerminal]"
            " Failed to parse JSON, was expecting an object.\n");
        return NULL;
    }

    struct Terminal* terminal = malloc(sizeof(struct Terminal));
    if (terminal == NULL)
    {
        fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
            " Failed to allocate memory for terminal.\n");
        return NULL;
    }

    // Loop through all the object keys
    for (size_t j = 0; j < t_json_val->u.object.length; ++j)
    {
        if (strcmp(t_json_val->u.object.values[j].name, JSON_CARD_TYPE_NAME) == 0)
        {
            json_value* t_json_card_types_val = t_json_val->u.object.values[j].value;
            if (parse_card_types(terminal, t_json_card_types_val) != 0)
            {
                Terminal_destroy(terminal);
                return NULL;
            }
        }
        else if (strcmp(t_json_val->u.object.values[j].name, JSON_TRANSACTION_TYPE_NAME) == 0)
        {
            json_value* t_json_trn_types_val = t_json_val->u.object.values[j].value;
            if (parse_transaction_types(terminal, t_json_trn_types_val) != 0)
            {
                Terminal_destroy(terminal);
                return NULL;
            }
        }
        else
        {
            Terminal_destroy(terminal);
            fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
                "Failed to parse JSON, unexpected key: %s.\n",
                t_json_val->u.object.values[j].name);
            return NULL;
        }
    }
    json_value_free(t_json_val);

    return terminal;
}

static int parse_card_types(struct Terminal* t, json_value* card_types_json)
{
    size_t num_card_types = card_types_json->u.array.length;
    t->card_types_sz = num_card_types;
    t->card_types = (int*)malloc(sizeof(int) * num_card_types);
    if (t->card_types == NULL)
    {
        fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
            "Failed to allocate memory for card types.\n");
        return -1;
    }

    for (size_t i = 0; i < num_card_types; ++i)
    {
        json_value* card_type_json = card_types_json->u.array.values[i];
        if (card_type_json->type != json_string)
        {
            fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
                "Failed to parse JSON, expected cardType array to contain strings.\n");
            return -1;
        }

        char* card_type = card_type_json->u.string.ptr;
        if (CardType_is_valid(card_type) != 0)
        {
            fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
                "Card type %s is not supported.\n", card_type);
            return -1;
        }
        t->card_types[i] = CardType_to_int(card_type);
    }
    return 0;
}

static int parse_transaction_types(struct Terminal* t, json_value* transaction_types_json)
{
    size_t num_transaction_types = transaction_types_json->u.array.length;
    t->transaction_types_sz = num_transaction_types;
    t->transaction_types = (int*)malloc(sizeof(int) * num_transaction_types);
    if (t->transaction_types == NULL)
    {
        fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
            "Failed to allocate memory for transaction types.\n");
        return -1;
    }

    for (size_t i = 0; i < num_transaction_types; ++i)
    {
        json_value* transaction_type_json = transaction_types_json->u.array.values[i];
        if (transaction_type_json->type != json_string)
        {
            fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
                "Failed to parse JSON, expected TransactionType array to contain strings.\n");
            return -1;
        }

        char* transaction_type = transaction_type_json->u.string.ptr;
        if (TransactionType_is_valid(transaction_type) != 0)
        {
            fprintf(stderr, "[TerminalJsonParser_ParseTerminals] "
                "Transaction type %s is not supported.\n", transaction_type);
            return -1;
        }
        t->transaction_types[i] = TransactionType_to_int(transaction_type);
    }
    return 0;
}
