#include "terminal_manager.h"

#include <stdlib.h>

static struct Terminal** terminals;
static size_t num_terminals = 0;
static size_t max_terminals = 10;

int TerminalManager_init()
{
    terminals = malloc(sizeof(struct Terminal*) * max_terminals);
    if (terminals == NULL)
    {
        return -1;
    }
    return 0;
}

void TerminalManager_add_terminal(struct Terminal* t)
{
    if (num_terminals + 1 == max_terminals)
    {
        max_terminals *= 2;
        terminals = realloc(terminals, sizeof(struct Terminal*) * max_terminals);
    }
    t->id = num_terminals;
    terminals[num_terminals++] = t;
}

struct Terminal* TerminalManager_get_terminal(int id)
{
    if (id >= 0 && id < num_terminals)
    {
        return terminals[id];
    }
    return NULL;
}

struct Terminal** TerminalManager_get_terminals()
{
    return terminals;
}

size_t TerminalManager_get_num_terminals()
{
    return num_terminals;
}

void TerminalManager_destroy()
{
    for (size_t i = 0; i < num_terminals; ++i)
    {
        Terminal_destroy(terminals[i]);
        num_terminals = 0;
    }
}
