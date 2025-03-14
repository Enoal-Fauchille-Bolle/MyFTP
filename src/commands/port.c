/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** PORT Command
*/

#include "myftp.h"

/**
 * @brief Parses host and port values from the PORT command argument
 * 
 * Splits the argument string into 6 integers representing the IP address
 * 
 * @param arg The command argument string
 * @param host_port Array to store the parsed host and port values
 * @return int 0 on success, -1 on error
 */
static int parse_host_port_values(char *arg, int *host_port)
{
    char *token = strtok(arg, ",");
    if (token == NULL) {
        return -1;
    }
    host_port[0] = atoi(token);

    for (int i = 1; i < 6; i++) {
        token = strtok(NULL, ",");
        if (token == NULL) {
            return -1;
        }
        host_port[i] = atoi(token);
    }
    return 0;
}

/**
 * @brief Parses host and port information from PORT command argument
 *
 * Splits the comma-separated argument into 6 integers representing
 * the IP address (4 numbers) and port (2 numbers).
 *
 * @param arg The command argument string
 * @return int* Array of 6 integers (must be freed by caller), NULL on error
 */
static int *get_host_port(char *arg)
{
    int *host_port = malloc(sizeof(int) * 6);

    if (host_port == NULL) {
        return NULL;
    }
    if (parse_host_port_values(arg, host_port) < 0) {
        free(host_port);
        return NULL;
    }

    return host_port;
}

/**
 * @brief Creates an active data socket for file transfer
 *
 * Sets up an active data socket using the provided host and port information.
 * Sends success or failure message to the client.
 *
 * @param connection The client connection structure
 * @param command The parsed command structure containing host/port info
 * @return command_status_t COMMAND_SUCCESS if socket created successfully,
 *                         COMMAND_FAILURE on error
 */
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
    printf("Active Data socket created on %d.%d.%d.%d:%d\n",
    host_port[0],
    host_port[1],
    host_port[2],
    host_port[3],
    connection->data_socket->port);
    dprintf(connection->client_sockfd,
    "200 PORT command successful. Consider using PASV.\r\n");
    free(host_port);
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the PORT FTP command
 *
 * Initiates active mode data transfer.
 * Requires user to be logged in and exactly one argument in h1,h2,h3,h4,p1,p2
 * format.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if active mode initialized
 * successfully, COMMAND_FAILURE otherwise
 */
command_status_t port_command(command_t *command, connection_t *connection)
{
    if (!connection->logged_in) {
        dprintf(
        connection->client_sockfd, "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(connection->client_sockfd, "500 Illegal PORT command.\r\n");
        return COMMAND_FAILURE;
    }
    return create_active_data_socket(connection, command);
}
