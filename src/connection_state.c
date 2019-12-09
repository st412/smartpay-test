#include "connection_state.h"

#include <stddef.h>

void ConnectionState_init(struct ConnectionState* s)
{
    s->request_body = NULL;
    s->response_body = NULL;
}
