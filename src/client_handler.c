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

int handle_connection(int sockfd, server_t *server)
{
    char *buffer = NULL;
    command_t command;
    command_status_t result;

    dprintf(sockfd, "220 Service ready for new user.\r\n");
    while (1) {
        buffer = read_socket(sockfd);
        printf("Received: %s", buffer);
        command = parse_buffer(buffer);
        result = execute_command(&command, server);
        if (buffer)
            free(buffer);
        else
            break;
        if (result == COMMAND_QUIT) {
            dprintf(sockfd, "221 Service closing control connection.\r\n");
            break;
        }
    }
    return 0;
}
