/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** STOR Command
*/

#include "myftp.h"

static int transfer_file_data(FILE *file, int client_sockfd)
{
    char buffer[1024];
    int read_bytes = read(client_sockfd, buffer, sizeof(buffer));

    while (read_bytes > 0) {
        read_bytes = read(client_sockfd, buffer, sizeof(buffer));
        fwrite(buffer, 1, read_bytes, file);
    }
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
    transfer_file_data(file, connection->data_socket->client_sockfd);
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
