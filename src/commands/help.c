/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** HELP Command
*/

#include "myftp.h"

/**
 * @brief Displays a list of available commands to the client
 *
 * This function sends a list of recognized commands to the client.
 * It checks if the user is logged in before sending the list.
 * If the user is not logged in, it sends an error message.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE
 * otherwise
 */
command_status_t help_command(command_t *command, connection_t *connection)
{
    (void)command;
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd,
        "214-The following commands are recognized.\r\n");
    dprintf(connection->client_sockfd,
        "USER PASS CWD CDUP QUIT DELE PWD PASV PORT HELP NOOP RETR STOR "
        "LIST\r\n");
    dprintf(connection->client_sockfd, "214 Help OK.\r\n");
    return COMMAND_SUCCESS;
}
