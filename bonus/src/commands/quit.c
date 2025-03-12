/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** QUIT Command
*/

#include "myftp.h"

command_status_t quit_command(command_t *command, client_t *client)
{
    char *response = NULL;

    (void)command;
    dprintf(client->sockfd, "QUIT\r\n");
    response = read_response(client->sockfd);
    if (!response) {
        fprintf(stderr, "Error reading response\n");
        return COMMAND_FAILURE;
    }
    if (strncmp(response, "221", 3) != 0) {
        fprintf(stderr, "Error: %s\n", response);
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    printf("Connection closed by server\n");
    return COMMAND_QUIT;
}
