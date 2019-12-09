#ifndef _TERMINAL_MANAGER_H
#define _TERMINAL_MANAGER_H

#include <stddef.h>

#include "terminal.h"

int TerminalManager_init();
void TerminalManager_add_terminal(struct Terminal* t);
struct Terminal* TerminalManager_get_terminal(int id);
struct Terminal** TerminalManager_get_terminals();
size_t TerminalManager_get_num_terminals();
void TerminalManager_destroy();

#endif
