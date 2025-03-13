/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Destroyers File
*/

#include "myftp.h"

/**
 * @brief Cleans up and destroys server resources
 *
 * Closes all client connections, frees allocated memory, and closes server
 * socket. Sends a closing message to all connected clients before
 * disconnecting.
 *
 * @param server The server structure to destroy
 * @param fds Array of poll file descriptors
 * @param connections Array of client connections
 */
void destroy_server(
    server_t *server, struct pollfd *fds, connection_t *connections)
{
    for (int i = 1; i < MAX_CLIENTS + 1; i++) {
        if (fds[i].fd >= 0) {
            dprintf(fds[i].fd, "221 Service closing control connection.\r\n");
            destroy_connection(&connections[i], true);
            close(fds[i].fd);
        }
    }
    free(server->path);
    close(server->sockfd);
}

/**
 * @brief Cleans up and destroys a data socket
 *
 * Closes the data connection socket and frees associated resources.
 * Handles both passive and active mode connections differently.
 * Prints status messages if verbose mode is enabled.
 *
 * @param data_socket The data socket structure to destroy
 * @param verbose Flag to enable/disable status messages
 */
void destroy_data_socket(data_socket_t *data_socket, bool verbose)
{
    if (data_socket == NULL)
        return;
    if (data_socket->data_socket_mode == PASSIVE) {
        if (verbose)
            printf(
                "Passive Data socket closed on port %d\n", data_socket->port);
    } else {
        if (verbose)
            printf("Active Data socket closed: %d.%d.%d.%d:%d\n",
                data_socket->address[0], data_socket->address[1],
                data_socket->address[2], data_socket->address[3],
                data_socket->port);
        free(data_socket->address);
    }
    if (data_socket->client_sockfd != -1)
        close(data_socket->client_sockfd);
    free(data_socket);
}

/**
 * @brief Cleans up and destroys a client connection
 *
 * Frees all resources associated with a client connection including
 * user data, working directory, client address, data socket, and stream.
 * Prints disconnection message if verbose mode is enabled.
 *
 * @param connection The connection structure to destroy
 * @param verbose Flag to enable/disable status messages
 */
void destroy_connection(connection_t *connection, bool verbose)
{
    if (connection == NULL)
        return;
    if (verbose)
        printf("Disconnected %s:%d\n",
            inet_ntoa(connection->client_addr->sin_addr),
            ntohs(connection->client_addr->sin_port));
    free(connection->user);
    if (connection->working_directory)
        free(connection->working_directory);
    free(connection->client_addr);
    if (connection->data_socket)
        destroy_data_socket(connection->data_socket, verbose);
    fclose(connection->stream);
}

/**
 * @brief Cleans up and destroys a command structure
 *
 * Frees all memory allocated for the command structure including
 * the command name and argument vector.
 *
 * @param command The command structure to destroy
 */
void destroy_command(command_t *command)
{
    char **tokens = NULL;

    if (command == NULL)
        return;
    tokens = command->argv - 1;
    free(command->name);
    for (int i = 0; i < command->argc && command->argv[i]; i++) {
        free(command->argv[i]);
    }
    free(tokens);
}
