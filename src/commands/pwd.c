/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** PWD Command
*/

#include "myftp.h"

/**
 * @brief Handles the PWD (Print Working Directory) FTP command
 *
 * Prints the current working directory path.
 * Requires user to be logged in.
 *
 * @param command The parsed command structure (unused)
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE
 * otherwise
 */
command_status_t pwd_command(command_t *command, connection_t *connection)
{
    (void)command;
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd, "257 \"%s\"\r\n",
        connection->working_directory);
    return COMMAND_SUCCESS;
}
