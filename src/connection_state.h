#ifndef _CONNECTION_STATE_H
#define _CONNECTION_STATE_H

struct ConnectionState
{
    char* request_body;
    char* response_body;
};

void ConnectionState_init(struct ConnectionState* s);

#endif
