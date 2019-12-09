CC = gcc
CFLAGS = -std=c99 -pedantic -Wall
OBJECTS = obj/util.o obj/json.o obj/connection_state.o obj/terminal_card_type.o obj/terminal_transaction_type.o obj/terminal.o obj/terminal_manager.o obj/terminal_json_generator.o obj/terminal_json_parser.o obj/main.o
INCLUDES = -I ./lib/libmicrohttpd/include
LIBRARIES = ./lib/libmicrohttpd/lib

all: smartpay-test-api

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $(INCLUDES) -o $@ $<

smartpay-test-api: $(OBJECTS)
	$(CC) $(OBJECTS) -L $(LIBRARIES) -lmicrohttpd -o bin/smartpay-test-api

clean:
	rm -f obj/*.o bin/smartpay-test-api