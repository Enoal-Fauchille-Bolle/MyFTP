/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** PASV Command
*/

#include "myftp.h"

/**
 * @brief Creates a passive data socket for file transfer
 *
 * Sets up a passive data socket and sends the connection information to the
 * client in the format required by FTP protocol (IP address and port number).
 *
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if socket created successfully,
 *                         COMMAND_FAILURE on error
 */
static command_status_t create_passive_data_socket(connection_t *connection)
{
    int *ports = NULL;

    connection->data_socket = setup_passive_data_socket();
    if (connection->data_socket == NULL) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    printf("Passive Data socket created on port %d\n",
        connection->data_socket->port);
    ports = split_port(connection->data_socket->port);
    dprintf(connection->client_sockfd,
        "227 Entering Passive Mode (%s,%d,%d).\r\n",
        replace_dots_with_commas(
            inet_ntoa(connection->data_socket->addr.sin_addr)),
        ports[0], ports[1]);
    free(ports);
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the PASV (Passive Mode) FTP command
 *
 * Initiates passive mode data transfer.
 * Requires user to be logged in.
 *
 * @param command The parsed command structure (unused)
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if passive mode initialized
 * successfully, COMMAND_FAILURE otherwise
 */
command_status_t pasv_command(command_t *command, connection_t *connection)
{
    (void)command;
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    return create_passive_data_socket(connection);
}
