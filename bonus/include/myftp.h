/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Main Header
*/

#ifndef MYFTP_H_
    #define MYFTP_H_

    #include <arpa/inet.h>
    #include <netinet/in.h>
    // #include <poll.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <string.h>
    #include <stdbool.h>
    // #include <dirent.h>
    #include <signal.h>
    #include <ctype.h>
    #include <errno.h>

typedef struct data_socket_s {
    int sockfd;
    struct sockaddr_in addr;
    int *address;
    int port;
} data_socket_t;

typedef struct client_s {
    int sockfd;
    int port;
    char *ip;
    data_socket_t *data_socket;
} client_t;

typedef struct command_s {
    char *name;
    int argc;
    char **argv;
} command_t;

typedef enum command_status_e {
    COMMAND_SUCCESS,
    COMMAND_FAILURE,
    COMMAND_NOT_FOUND,
    COMMAND_QUIT,
} command_status_t;

typedef struct command_handler_s {
    char *command_name;
    command_status_t (*handler)(command_t *command, client_t *client);
} command_handler_t;

// Client
int client(char *ip, int port);

// Socket
struct sockaddr_in init_sockin(char *ip, int port);

// Reading
char *read_input(void);
char *read_response(int sockfd);

// Login
int login(client_t *client);

// Command Parser
command_t *parse_buffer(char *buffer);

// Command Executor
command_status_t execute_command(command_t *command, client_t *client);

// Utilities
char *touppercase(char *str);
char *trim(char *str);
int merge_port(int ports[2]);

// Destroyers
void destroy_client(client_t *client);
void destroy_command(command_t *command);
void destroy_data_socket(data_socket_t *data_socket);

// Signal
void setup_signal(void);
bool is_client_stopped(void);

// Data Socket
data_socket_t *create_data_socket(client_t *client);
command_status_t connect_data_socket(data_socket_t *data_socket);

// Commands
command_status_t quit_command(command_t *command, client_t *client);
command_status_t exit_command(command_t *command, client_t *client);
command_status_t help_command(command_t *command, client_t *client);
command_status_t ls_command(command_t *command, client_t *client);
command_status_t pwd_command(command_t *command, client_t *client);
command_status_t cd_command(command_t *command, client_t *client);
command_status_t get_command(command_t *command, client_t *client);
command_status_t put_command(command_t *command, client_t *client);

#endif /* !MYFTP_H_ */
