/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** DELE Command
*/

#include "myftp.h"

static command_status_t delete_file(connection_t *connection, char *filepath)
{
    chdir(connection->working_directory);
    if (remove(filepath) != 0) {
        dprintf(connection->client_sockfd, "550 Delete operation failed.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd, "250 Requested file action okay.\r\n");
    return COMMAND_SUCCESS;
}

command_status_t dele_command(command_t *command, connection_t *connection)
{
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(connection->client_sockfd, "550 Delete operation failed.\r\n");
        return COMMAND_FAILURE;
    }
    return delete_file(connection, command->argv[0]);
}
