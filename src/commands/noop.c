/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** NOOP Command
*/

#include "myftp.h"

command_status_t noop_command(command_t *command, connection_t *connection)
{
    (void)command;
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd, "200 NOOP ok.\r\n");
    return COMMAND_SUCCESS;
}
