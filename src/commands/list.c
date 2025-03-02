/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** LIST Command
*/

#include "myftp.h"

static command_status_t execute_ls_command(
    connection_t *connection, command_t *command)
{
    FILE *fp = NULL;
    char buffer[1024];

    (void)command;
    dprintf(connection->client_sockfd,
        "150 Here comes the directory listing.\r\n");
    chdir(connection->working_directory);
    fp = popen("ls -l", "r");
    chdir(connection->server->path);
    if (fp == NULL) {
        perror("popen");
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        dprintf(connection->data_socket->client_sockfd, "%s", buffer);
    dprintf(connection->client_sockfd, "226 Directory send OK.\r\n");
    pclose(fp);
    return COMMAND_SUCCESS;
}

command_status_t list_command(command_t *command, connection_t *connection)
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
    return execute_data_socket_command(
        connection, execute_ls_command, command);
}
