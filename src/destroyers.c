/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Destroyers File
*/

#include "myftp.h"

void destroy_server(
    server_t *server, struct pollfd *fds, connection_t *connections)
{
    for (int i = 1; i < MAX_CLIENTS + 1; i++) {
        if (fds[i].fd >= 0) {
            dprintf(fds[i].fd, "221 Service closing control connection.\r\n");
            destroy_connection(&connections[i]);
            close(fds[i].fd);
        }
    }
    free(server->path);
    close(server->sockfd);
}

void destroy_connection(connection_t *connection)
{
    free(connection->user);
    free(connection->working_directory);
    free(connection->client_addr);
    if (connection->data_socket) {
        close(connection->data_socket->data_sockfd);
        free(connection->data_socket);
    }
    fclose(connection->stream);
}

void destroy_command(command_t *command)
{
    char **tokens =
        command->argv - 1;

    free(command->name);
    for (int i = 0; i < command->argc && command->argv[i]; i++) {
        free(command->argv[i]);
    }
    free(tokens);
}
