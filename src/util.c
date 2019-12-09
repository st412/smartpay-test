#include "util.h"

#include <string.h>

int str_start_with(const char *str, const char *str_start_with)
{
    size_t lenstr = strlen(str), lenstr_start_with = strlen(str_start_with);
    if (lenstr_start_with < lenstr)
    {
        return -1;
    }
    if (memcmp(str_start_with, str, lenstr_start_with) == 0)
    {
        return 0;
    }
    return -1;
}
