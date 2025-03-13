/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** NOOP Command
*/

#include "myftp.h"

/**
 * @brief Handles the NOOP (No Operation) FTP command
 *
 * Does nothing except check if user is logged in and return a success message.
 * Requires user to be logged in.
 *
 * @param command The parsed command structure (unused)
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE
 * otherwise
 */
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
