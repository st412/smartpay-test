#ifndef _TERMINAL_JSON_GENERATOR_H
#define _TERMINAL_JSON_GENERATOR_H

#include "terminal.h"

char* TerminalJsonGenerator_generate(struct Terminal** ts, size_t ts_sz);
char* TerminalJsonGenerator_generate_one(struct Terminal* t);

#endif
