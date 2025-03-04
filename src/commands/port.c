/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** PORT Command
*/

#include "myftp.h"

static int *get_host_port(char *arg)
{
    int *host_port = malloc(sizeof(int) * 6);

    host_port[0] = atoi(strtok(arg, ","));
    for (int i = 1; i < 6; i++) {
        host_port[i] = atoi(strtok(NULL, ","));
    }
    return host_port;
}

static command_status_t create_active_data_socket(
    connection_t *connection, command_t *command)
{
    int *host_port = get_host_port(command->argv[0]);

    if (host_port == NULL) {
        dprintf(connection->client_sockfd, "500 Illegal PORT command.\r\n");
        return COMMAND_FAILURE;
    }
    connection->data_socket = setup_active_data_socket(host_port);
    if (connection->data_socket == NULL) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    printf("Active Data socket created on %d.%d.%d.%d:%d\n", host_port[0],
        host_port[1], host_port[2], host_port[3],
        connection->data_socket->port);
    dprintf(connection->client_sockfd,
        "200 PORT command successful. Consider using PASV.\r\n");
    free(host_port);
    return COMMAND_SUCCESS;
}

command_status_t port_command(command_t *command, connection_t *connection)
{
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(connection->client_sockfd, "500 Illegal PORT command.\r\n");
        return COMMAND_FAILURE;
    }
    return create_active_data_socket(connection, command);
}
