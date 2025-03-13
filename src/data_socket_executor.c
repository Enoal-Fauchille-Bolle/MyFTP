/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Data Socket Executor
*/

#include "myftp.h"

/**
 * @brief Executes a command in a forked process
 *
 * Creates a child process to execute the command, allowing for parallel
 * data transfer operations. Handles cleanup of resources in child process.
 *
 * @param connection Client connection structure
 * @param command_execution Function pointer to the command handler
 * @param command Command structure to execute
 * @return COMMAND_SUCCESS on successful fork, COMMAND_FAILURE on error
 */
static command_status_t fork_execution(connection_t *connection,
    command_status_t (*command_execution)(connection_t *, command_t *),
    command_t *command)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return COMMAND_FAILURE;
    }
    if (pid == 0) {
        command_execution(connection, command);
        destroy_command(command);
        free(connection->server->path);
        close(connection->server->sockfd);
        destroy_connection(connection, false);
        exit(0);
    }
    return COMMAND_SUCCESS;
}

/**
 * @brief Accepts an incoming connection on a passive data socket
 *
 * Waits for and accepts a client connection on the passive mode data socket.
 * Prints connection information when successful.
 *
 * @param data_socket Data socket structure
 * @return 0 on success, -1 on error
 */
static int accept_connection(data_socket_t *data_socket)
{
    socklen_t addr_len = sizeof(data_socket->addr);

    data_socket->client_sockfd = accept(data_socket->data_sockfd,
        (struct sockaddr *)&data_socket->addr, &addr_len);
    if (data_socket->client_sockfd == -1) {
        perror("accept");
        return -1;
    }
    printf("Passive Data socket connection accepted from %s:%d\n",
        inet_ntoa(data_socket->addr.sin_addr),
        ntohs(data_socket->addr.sin_port));
    return 0;
}

/**
 * @brief Establishes connection for active mode data socket
 *
 * Attempts to connect to the client's specified address and port
 * for active mode data transfer.
 *
 * @param data_socket Data socket structure
 * @return 0 on success, -1 on error
 */
static int active_connection(data_socket_t *data_socket)
{
    if (connect(data_socket->client_sockfd,
            (struct sockaddr *)&data_socket->addr,
            sizeof(data_socket->addr)) == -1) {
        perror("connect");
        return -1;
    }
    return 0;
}

/**
 * @brief Establishes data socket connection based on transfer mode
 *
 * Handles connection establishment for both active and passive mode
 * data transfers using the appropriate connection method.
 *
 * @param data_socket Data socket structure
 * @return 0 on success, -1 on error
 */
static int establish_data_socket_connection(data_socket_t *data_socket)
{
    if (data_socket->data_socket_mode == PASSIVE) {
        if (accept_connection(data_socket) == -1)
            return -1;
    } else {
        if (active_connection(data_socket) == -1)
            return -1;
    }
    return 0;
}

/**
 * @brief Executes a command that requires data socket connection
 *
 * Sets up data connection, executes the command in a child process,
 * and handles cleanup of data socket resources.
 *
 * @param connection Client connection structure
 * @param command_execution Function pointer to the command handler
 * @param command Command structure to execute
 * @return COMMAND_SUCCESS on successful execution, COMMAND_FAILURE on error
 */
command_status_t execute_data_socket_command(connection_t *connection,
    command_status_t (*command_execution)(connection_t *, command_t *),
    command_t *command)
{
    if (establish_data_socket_connection(connection->data_socket) == -1) {
        dprintf(connection->client_sockfd,
            "425 Failed to establish connection.\r\n");
        destroy_data_socket(connection->data_socket, true);
        connection->data_socket = NULL;
        return COMMAND_FAILURE;
    }
    if (fork_execution(connection, command_execution, command) ==
        COMMAND_FAILURE) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
    }
    destroy_data_socket(connection->data_socket, true);
    connection->data_socket = NULL;
    return COMMAND_SUCCESS;
}
