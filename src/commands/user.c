/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

command_status_t user_command(command_t *command, connection_t *connection)
{
    if (connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Can't change to another user.\r\n");
        return COMMAND_FAILURE;
    }
    if (connection->user)
        free(connection->user);
    if (command->argc == 0) {
        connection->user = NULL;
    } else {
        connection->user = strdup(command->argv[0]);
    }
    dprintf(connection->client_sockfd, "331 Please specify the password.\r\n");
    return COMMAND_SUCCESS;
}
