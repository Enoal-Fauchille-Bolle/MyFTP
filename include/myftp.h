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
    #include <dirent.h>

typedef struct server_s {
    int sockfd;
    struct sockaddr_in addr;
    int port;
    char *path;
} server_t;

typedef struct data_socket_s {
    int sockfd;
    struct sockaddr_in addr;
    int port;
} data_socket_t;

typedef struct connection_s {
    server_t *server;
    int client_sockfd;
    struct sockaddr_in *client_addr;
    FILE *stream;
    bool logged_in;
    char *user;
    char *working_directory;
    data_socket_t *data_socket;
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
    COMMAND_QUIT,
} command_status_t;

typedef struct command_handler_s {
    char *command_name;
    command_status_t (*handler)(command_t *command, connection_t *connection);
} command_handler_t;


// Client Handler
void handle_connection(struct pollfd *fd, connection_t *connection);

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
int merge_port(int ports[2]);
int *split_port(int port);
char *replace_dots_with_commas(char *string);

// Destroyers
void destroy_connection(connection_t *connection);
void destroy_command(command_t *command);
void destroy_server(
    server_t *server, struct pollfd *fds, connection_t *connections);

// Data Socket
data_socket_t setup_data_socket(void);

// Commands
command_status_t user_command(command_t *command, connection_t *connection);
command_status_t pass_command(command_t *command, connection_t *connection);
command_status_t cwd_command(command_t *command, connection_t *connection);
command_status_t cdup_command(command_t *command, connection_t *connection);
command_status_t quit_command(command_t *command, connection_t *connection);
command_status_t dele_command(command_t *command, connection_t *connection);
command_status_t pwd_command(command_t *command, connection_t *connection);
command_status_t pasv_command(command_t *command, connection_t *connection);
command_status_t port_command(command_t *command, connection_t *connection);
command_status_t help_command(command_t *command, connection_t *connection);
command_status_t noop_command(command_t *command, connection_t *connection);
command_status_t retr_command(command_t *command, connection_t *connection);
command_status_t stor_command(command_t *command, connection_t *connection);
command_status_t list_command(command_t *command, connection_t *connection);

#endif /* !MYFTP_H_ */
