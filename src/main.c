#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#include "connection_state.h"
#include "terminal_json_parser.h"
#include "terminal_json_generator.h"
#include "terminal_manager.h"
#include "util.h"

#define PORT 8181
#define POST_BUFFER_SIZE 4096

int answer_to_connection(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method, const char *version,
    const char *upload_data, size_t *upload_data_size, void **con_cls);

void request_completed(void *cls, struct MHD_Connection *connection, void **con_cls,
    enum MHD_RequestTerminationCode toe);

static int send_json(struct MHD_Connection *connection, const char* json,
    int status_code);

int main()
{
    if (TerminalManager_init() != 0)
    {
        fprintf(stderr, "[main] Failed to initialize TerminalManager\n");
        return -1;
    }

    unsigned int daemon_options = MHD_USE_INTERNAL_POLLING_THREAD | 
        MHD_USE_PEDANTIC_CHECKS;

    struct MHD_Daemon *daemon = MHD_start_daemon(daemon_options, PORT, NULL, NULL,
        &answer_to_connection, NULL, MHD_OPTION_NOTIFY_COMPLETED, &request_completed,
        NULL, MHD_OPTION_END);
    if (daemon == NULL) return 1;

    // Wait
    getchar();

    // Cleanup
    MHD_stop_daemon(daemon);
    TerminalManager_destroy();

    return 0;
}

int answer_to_connection(void *cls, struct MHD_Connection *connection,
    const char *url, const char *method, const char *version,
    const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    if (strcmp(method, "GET") == 0 && strcmp(url, "/terminals") == 0)
    {
        struct ConnectionState* state = malloc(sizeof(struct ConnectionState));
        if (state == NULL)
        {
            // TODO: Should write an error description
            return send_json(connection, "", 500);
        }
        ConnectionState_init(state);

        const char* id = MHD_lookup_connection_value(connection, 
            MHD_GET_ARGUMENT_KIND, "id");
        if (id != NULL)
        {
            int id_i = strtol(id, NULL, 10);
            if (errno != 0)
            {
                return send_json(connection, "", 400);
            }

            struct Terminal* terminal = TerminalManager_get_terminal(id_i);
            if (terminal == NULL)
            {
                return send_json(connection, "", 404);
            }

            char* json = TerminalJsonGenerator_generate_one(terminal);
            state->response_body = json;
            *con_cls = state;

            return send_json(connection, json, 200);
        }

        struct Terminal** terminals = TerminalManager_get_terminals();
        size_t terminals_sz = TerminalManager_get_num_terminals();
        char* json = TerminalJsonGenerator_generate(terminals, terminals_sz);

        state->response_body = json;
        *con_cls = state;

        return send_json(connection, json, 200);
    }

    if (strcmp(method, "POST") == 0) 
    {
        if (strcmp(url, "/terminals") != 0)
        {
            return send_json(connection, "", 404);   
        }

        // If this is the first post chunk, set the state of the request to hold
        // the connection info
        if (*con_cls == NULL)
        {
            struct ConnectionState* state = malloc(sizeof(struct ConnectionState));
            if (state == NULL)
            {
                fprintf(stderr, "[answer_to_connection] Failed to allocate "
                    "memory for the connection state.\n");
                return send_json(connection, "", 500);  
            }
            ConnectionState_init(state);

            state->request_body = malloc(POST_BUFFER_SIZE);
            memset(state->request_body, 0, POST_BUFFER_SIZE);
            if (state->request_body == NULL)
            {
                fprintf(stderr, "[answer_to_connection] Failed to allocate "
                    "memory for the request body.\n");
                return send_json(connection, "", 500);  
            }

            *con_cls = state;
            return MHD_YES;
        }

        // Get the current state of the request
        struct ConnectionState* state = *con_cls;
        
        // If we got all the request data, handle the request
        if (*upload_data_size == 0)
        {
            struct Terminal* terminal = TerminalJsonParser_ParseTerminal(state->request_body);
            if (terminal == NULL)
            {
                return send_json(connection, "", 400);
            }

            TerminalManager_add_terminal(terminal);
            return send_json(connection, state->request_body, 200);
        }
        else
        {
            strcat(state->request_body, upload_data);
            *upload_data_size -= strlen(upload_data);
            return MHD_YES;
        }
    }

    return send_json(connection, "", 404);
}

void request_completed(void *cls, struct MHD_Connection *connection, void **con_cls,
    enum MHD_RequestTerminationCode toe)
{
    // Get the state
    struct ConnectionState* state = *con_cls;
    if (state == NULL)
    {
        return;
    }

    // Cleanup
    if (state->request_body != NULL)
    {
        free(state->request_body);
    }
    if (state->response_body != NULL)
    {
        free(state->response_body);
    }
    free(state);
    *con_cls = NULL;
}

static int send_json(struct MHD_Connection *connection, const char* json,
    int status_code)
{
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(json),
        (void*)json, MHD_RESPMEM_PERSISTENT);
    if (!response)
    {
        return MHD_NO;
    }

    MHD_add_response_header(response, "Content-Type", "application/json");
 
    int ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    
    return ret;
}
