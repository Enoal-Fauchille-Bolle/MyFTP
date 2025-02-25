/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Main Header
*/

#ifndef MYFTP_H_
    #define MYFTP_H_

    #define MAX_CLIENTS 128
    #define BUFFER_SIZE 1024
    #define POLL_TIMEOUT 10000

    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <string.h>
    #include <stdbool.h>

typedef struct server_s {
    int server_sockfd;
    struct sockaddr_in addr;
    int port;
    char *path;
} server_t;

typedef struct connection_s {
    int client_sockfd;
    bool logged_in;
    server_t *server;
} connection_t;

typedef struct command_s {
    char *name;
    int argc;
    char **argv;
} command_t;

typedef enum command_status {
    COMMAND_SUCCESS,
    COMMAND_FAILURE,
    COMMAND_NOT_FOUND,
    COMMAND_QUIT
} command_status_t;

typedef struct command_handler_s {
    char *command_name;
    command_status_t (*handler)(command_t *command, connection_t *connection);
} command_handler_t;

// Client Handler
int handle_connection(int sockfd, server_t *server);

// Socket
server_t setup_socket(int port, char *path);

// Connection
int process_connections(server_t *server);

// Command Parser
command_t parse_buffer(char *buffer);

// Command Executor
command_status_t execute_command(command_t *command, connection_t *connection);

// Utils
char *touppercase(char *str);

// Commands
command_status_t user_command(command_t *command, server_t *server);
command_status_t quit_command(command_t *command, server_t *server);

#endif /* !MYFTP_H_ */
