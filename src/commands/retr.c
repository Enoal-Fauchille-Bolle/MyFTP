/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** RETR Command
*/

#include "myftp.h"

static int check_file_exists(connection_t *connection, command_t *command)
{
    FILE *fp = NULL;

    chdir(connection->working_directory);
    fp = fopen(command->argv[0], "r");
    chdir(connection->server->path);
    if (fp == NULL && access(command->argv[0], R_OK) != 0) {
        dprintf(connection->client_sockfd,
            "550 Permission denied.\r\n");
        return -1;
    }
    if (fp == NULL) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return -1;
    }
    fclose(fp);
    return 0;
}

static command_status_t execute_retr_command(
    connection_t *connection, command_t *command)
{
    FILE *fp = NULL;
    char buffer[1024];

    dprintf(connection->client_sockfd,
        "150 Opening BINARY mode data connection.\r\n");
    chdir(connection->working_directory);
    fp = fopen(command->argv[0], "r");
    chdir(connection->server->path);
    if (fp == NULL) {
        perror("fopen");
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        dprintf(connection->data_socket->client_sockfd, "%s", buffer);
    dprintf(connection->client_sockfd, "226 Transfer complete.\r\n");
    fclose(fp);
    return COMMAND_SUCCESS;
}

command_status_t retr_command(command_t *command, connection_t *connection)
{
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
    if (check_file_exists(connection, command) == -1)
        return COMMAND_FAILURE;
    return execute_data_socket_command(
        connection, execute_retr_command, command);
}
