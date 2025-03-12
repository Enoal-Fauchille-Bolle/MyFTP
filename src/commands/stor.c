/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** STOR Command
*/

#include "myftp.h"

static int transfer_file_data(FILE *file, int client_sockfd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_bytes;
    FILE *client_stream = fdopen(client_sockfd, "r");

    if (client_stream == NULL)
        return -1;
    while ((read_bytes = getline(&line, &len, client_stream)) > 0) {
        fwrite(line, 1, read_bytes, file);
    }

    free(line);
    fclose(client_stream);
    return read_bytes;
}

static command_status_t execute_stor_command(
    connection_t *connection, command_t *command)
{
    FILE *file = fopen(command->argv[0], "wb");

    dprintf(connection->client_sockfd,
        "150 Opening BINARY mode data connection.\r\n");
    if (file == NULL) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    puts("a");
    transfer_file_data(file, connection->data_socket->client_sockfd);
    puts("b");
    dprintf(connection->client_sockfd, "226 Transfer complete.\r\n");
    fclose(file);
    return COMMAND_SUCCESS;
}

command_status_t stor_command(command_t *command, connection_t *connection)
{
    (void)command;
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (connection->data_socket == NULL) {
        dprintf(connection->client_sockfd, "425 Use PASV or PORT first.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(connection->client_sockfd, "550 Failed to open file.\r\n");
        return COMMAND_FAILURE;
    }
    return execute_data_socket_command(
        connection, execute_stor_command, command);
}
