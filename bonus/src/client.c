/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Client
*/

#include "myftp.h"

static client_t *init_client(int sockfd, char *ip, int port)
{
    client_t *client = malloc(sizeof(client_t));

    if (!client) {
        perror("malloc");
        return NULL;
    }
    client->sockfd = sockfd;
    client->ip = strdup(ip);
    client->port = port;
    return client;
}

static client_t *connect_to_server(char *ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = init_sockin(ip, port);
    socklen_t addr_len = 0;

    if (sockfd == -1) {
        perror("socket");
        return NULL;
    }
    addr_len = sizeof(addr);
    if (connect(sockfd, (struct sockaddr *)&addr, addr_len) == -1) {
        perror("connect");
        return NULL;
    }
    return init_client(sockfd, ip, port);
}

static command_status_t loop_execution(client_t *client)
{
    char *buffer = NULL;
    command_t *command = NULL;
    command_status_t last_command = COMMAND_SUCCESS;

    printf("myftp> ");
    buffer = read_input();
    if (!buffer) {
        return COMMAND_QUIT;
    }
    command = parse_buffer(buffer);
    if (!command) {
        free(buffer);
        return COMMAND_QUIT;
    }
    last_command = execute_command(command, client);
    free(buffer);
    destroy_command(command);
    return last_command;
}

int client(char *ip, int port)
{
    client_t *client = NULL;
    command_status_t last_command = COMMAND_SUCCESS;

    setup_signal();
    printf("Connecting to server at %s:%d\n", ip, port);
    client = connect_to_server(ip, port);
    if (!client)
        return 84;
    if (login(client) == 84) {
        destroy_client(client);
        puts("\nExiting...");
        return 84;
    }
    while (last_command != COMMAND_QUIT && !is_client_stopped()) {
        last_command = loop_execution(client);
    }
    puts("\nExiting...");
    destroy_client(client);
    return 0;
}
