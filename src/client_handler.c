/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Client Handler File
*/

#include "myftp.h"

static void destroy_command(command_t *command)
{
    free(command->name);
    for (int i = 0; i < command->argc && command->argv[i]; i++) {
        free(command->argv[i]);
    }
}

static char *read_socket(int client_sockfd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fdopen(client_sockfd, "r");

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

static int handle_client_command(connection_t *connection)
{
    command_status_t result;
    char *buffer = NULL;
    command_t command;

    buffer = read_socket(connection->client_sockfd);
    if (!buffer)
        return 1;
    printf("Executing command: %s\n", buffer);
    command = parse_buffer(buffer);
    result = execute_command(&command, connection);
    destroy_command(&command);
    free(buffer);
    if (result == COMMAND_QUIT)
        return 1;
    return 0;
}

static void destroy_connection(connection_t *connection)
{
    free(connection->user);
    free(connection->working_directory);
}

int handle_connection(int client_sockfd, connection_t *connection)
{
    int result = 0;

    dprintf(client_sockfd, "220 Service ready for new user.\r\n");
    while (1) {
        result = handle_client_command(connection);
        if (result) {
            dprintf(
                client_sockfd, "221 Service closing control connection.\r\n");
            break;
        }
    }
    printf("Disconnected %s:%d\n",
        inet_ntoa(connection->client_addr->sin_addr),
        ntohs(connection->client_addr->sin_port));
    destroy_connection(connection);
    return 0;
}
