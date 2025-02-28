/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

static int accept_connection(data_socket_t *data_socket)
{
    socklen_t addr_len = sizeof(data_socket->addr);

    data_socket->sockfd = accept(
        data_socket->sockfd, (struct sockaddr *)&data_socket->addr, &addr_len);
    if (data_socket->sockfd == -1) {
        perror("accept");
        return -1;
    }
    printf("Data connection accepted from %s:%d\n",
        inet_ntoa(data_socket->addr.sin_addr),
        ntohs(data_socket->addr.sin_port));
    return 0;
}

static char *get_ls_command(char *working_directory)
{
    int length = 7 + strlen(working_directory);
    char *cmd = malloc(sizeof(char) * (length));

    if (cmd == NULL) {
        perror("malloc");
        return NULL;
    }
    snprintf(cmd, length, "ls -l %s", working_directory);
    return cmd;
}

static int execute_ls_command(char *cmd, connection_t *connection)
{
    FILE *fp = popen(cmd, "r");
    char buffer[1024];

    free(cmd);
    if (fp == NULL) {
        perror("popen");
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return -1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        dprintf(connection->data_socket->sockfd, "%s", buffer);
    }
    pclose(fp);
    return 0;
}

static command_status_t list(connection_t *connection)
{
    char *cmd = get_ls_command(connection->working_directory);

    if (cmd == NULL || accept_connection(connection->data_socket) == -1) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd,
        "150 Here comes the directory listing.\r\n");
    if (execute_ls_command(cmd, connection) == -1) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd, "226 Directory send OK.\r\n");
    close(connection->data_socket->sockfd);
    free(connection->data_socket);
    connection->data_socket = NULL;
    return COMMAND_SUCCESS;
}

command_status_t list_command(command_t *command, connection_t *connection)
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
    dprintf(connection->client_sockfd, "502 Command not implemented.\r\n");
    return list(connection);
}
