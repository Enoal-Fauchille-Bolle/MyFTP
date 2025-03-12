/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Destroyers
*/

#include "myftp.h"

void destroy_client(client_t *client)
{
    if (!client)
        return;
    if (client->sockfd != -1) {
        dprintf(client->sockfd, "QUIT\r\n");
        close(client->sockfd);
    }
    free(client->ip);
    free(client);
}

void destroy_command(command_t *command)
{
    char **tokens = NULL;

    if (command == NULL)
        return;
    tokens = command->argv - 1;
    free(command->name);
    for (int i = 0; i < command->argc && command->argv[i]; i++) {
        free(command->argv[i]);
    }
    free(tokens);
    free(command);
}

void destroy_data_socket(data_socket_t *data_socket)
{
    if (data_socket == NULL)
        return;
    free(data_socket->address);
    if (data_socket->sockfd != -1)
        close(data_socket->sockfd);
    free(data_socket);
}
