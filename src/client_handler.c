/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Client Handler File
*/

#include "myftp.h"

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

static command_status_t handle_client_command(connection_t *connection)
{
    command_status_t result = COMMAND_NOT_FOUND;
    char *buffer = NULL;
    command_t command = {0};

    buffer = read_socket(connection);
    if (!buffer)
        return 1;
    printf("Command from %s:%d: %s\n",
        inet_ntoa(connection->client_addr->sin_addr),
        ntohs(connection->client_addr->sin_port), buffer);
    command = parse_buffer(buffer);
    free(buffer);
    result = execute_command(&command, connection);
    destroy_command(&command);
    return result;
}

void handle_connection(struct pollfd *fd, connection_t *connection)
{
    command_status_t result = handle_client_command(connection);

    if (result == COMMAND_QUIT) {
        dprintf(fd->fd, "221 Service closing control connection.\r\n");
        printf("Disconnected %s:%d\n",
            inet_ntoa(connection->client_addr->sin_addr),
            ntohs(connection->client_addr->sin_port));
        destroy_connection(connection, true);
        close(fd->fd);
        fd->fd = -1;
    }
}
