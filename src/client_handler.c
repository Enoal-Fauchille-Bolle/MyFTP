/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Client Handler File
*/

#include "myftp.h"

static char *read_socket(int sockfd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fdopen(sockfd, "r");

    if (fp == NULL) {
        perror("fdopen");
        return NULL;
    }
    read = getline(&line, &len, fp);
    if (read == -1) {
        free(line);
        fclose(fp);
        return NULL;
    }
    if (line[read - 1] == '\n')
        line[read - 1] = '\0';
    return line;
}

int handle_client_command(connection_t *connection)
{
    command_status_t result;
    char *buffer = NULL;
    command_t command;

    buffer = read_socket(connection->client_sockfd);
    if (!buffer)
        return 1;
    command = parse_buffer(buffer);
    result = execute_command(&command, connection);
    if (buffer)
        free(buffer);
    else
        return 1;
    if (result == COMMAND_QUIT)
        return 1;
    return 0;
}

int handle_connection(int client_sockfd, server_t *server)
{
    int result = 0;
    connection_t connection = {client_sockfd, false, server};

    dprintf(client_sockfd, "220 Service ready for new user.\r\n");
    while (1) {
        result = handle_client_command(&connection);
        if (result) {
            dprintf(
                client_sockfd, "221 Service closing control connection.\r\n");
            break;
        }
    }
    return 0;
}
