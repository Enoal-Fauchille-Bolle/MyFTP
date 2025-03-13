/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Client Handler File
*/

#include "myftp.h"

/**
 * @brief Reads a line from the client socket stream
 *
 * Reads input from the client connection stream until a newline is
 * encountered. Removes the trailing newline character if present.
 *
 * @param connection The client connection structure containing the input
 * stream
 * @return Pointer to the read line string, NULL if read fails or connection
 * closed
 */
static char *read_socket(connection_t *connection)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    read = getline(&line, &len, connection->stream);
    if (read == -1) {
        free(line);
        return NULL;
    }
    if (line[read - 1] == '\n')
        line[read - 1] = '\0';
    return line;
}

/**
 * @brief Processes a single client command
 *
 * Reads a command from the client, parses it, executes it, and cleans up.
 * Prints the received command for logging purposes.
 *
 * @param connection The client connection structure
 * @return command_status_t indicating the result of command execution
 */
static command_status_t handle_client_command(connection_t *connection)
{
    command_status_t result = COMMAND_NOT_FOUND;
    char *buffer = NULL;
    command_t *command = NULL;

    buffer = read_socket(connection);
    if (!buffer)
        return 1;
    printf("Command from %s:%d: %s\n",
        inet_ntoa(connection->client_addr->sin_addr),
        ntohs(connection->client_addr->sin_port), buffer);
    command = parse_buffer(buffer);
    free(buffer);
    result = execute_command(command, connection);
    destroy_command(command);
    return result;
}

/**
 * @brief Handles an active client connection
 *
 * Processes client commands and manages connection state. Handles
 * disconnection when the client sends a QUIT command or when the connection is
 * lost.
 *
 * @param fd The poll file descriptor for the client socket
 * @param connection The client connection structure
 */
void handle_connection(struct pollfd *fd, connection_t *connection)
{
    command_status_t result = handle_client_command(connection);

    if (result == COMMAND_QUIT) {
        dprintf(fd->fd, "221 Service closing control connection.\r\n");
        destroy_connection(connection, true);
        close(fd->fd);
        fd->fd = -1;
    }
}
