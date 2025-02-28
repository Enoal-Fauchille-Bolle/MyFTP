/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

static void data_socket(connection_t *connection)
{
    data_socket_t *data_socket = malloc(sizeof(data_socket_t));
    int *ports = NULL;

    if (data_socket == NULL) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return;
    }
    *data_socket = setup_data_socket();
    printf("Data socket created on port %d\n", data_socket->port);
    ports = split_port(data_socket->port);
    dprintf(connection->client_sockfd,
        "227 Entering Passive Mode (%s,%d,%d).\r\n",
        replace_dots_with_commas(inet_ntoa(data_socket->addr.sin_addr)),
        ports[0], ports[1]);
    free(ports);
    connection->data_socket = data_socket;
}

command_status_t pasv_command(command_t *command, connection_t *connection)
{
    (void)command;
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    data_socket(connection);
    return COMMAND_SUCCESS;
}
