/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** TYPE Command
*/

#include "myftp.h"

/**
 * @brief Changes the transfer type mode
 *
 * Switches between ASCII and Binary transfer modes.
 * Accepts "A" for ASCII mode or "I" for Binary mode.
 * Sends appropriate response message to client.
 *
 * @param type The type string to set ("A" or "I")
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if type changed successfully,
 *                         COMMAND_FAILURE if type is invalid
 */
static command_status_t change_type(char *type, connection_t *connection)
{
    type = touppercase(type);
    if (strcmp(type, "A") == 0) {
        connection->transfer_mode = ASCII;
        dprintf(connection->client_sockfd, "200 Switching to ASCII mode.\r\n");
    } else if (strcmp(type, "I") == 0) {
        connection->transfer_mode = BINARY;
        dprintf(
            connection->client_sockfd, "200 Switching to Binary mode.\r\n");
    } else {
        dprintf(
            connection->client_sockfd, "500 Unrecognised TYPE command.\r\n");
        return COMMAND_FAILURE;
    }
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the TYPE FTP command
 *
 * Sets the type of file transfer (ASCII or Binary).
 * Requires user to be logged in and exactly one argument.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if type changed successfully,
 *                         COMMAND_FAILURE otherwise
 */
command_status_t type_command(command_t *command, connection_t *connection)
{
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(
            connection->client_sockfd, "500 Unrecognised TYPE command.\r\n");
        return COMMAND_FAILURE;
    }
    return change_type(command->argv[0], connection);
}
