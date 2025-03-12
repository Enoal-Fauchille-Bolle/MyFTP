/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** PWD Command
*/

#include "myftp.h"

static void display_current_path(char *response)
{
    char *path = strchr(response, '"');
    char *end = NULL;

    if (path) {
        path++;
        end = strchr(path, '"');
        if (end) {
            *end = '\0';
        }
        printf("Current directory: %s\n", path);
    } else {
        fprintf(stderr, "Invalid response format: %s\n", response);
    }
}

command_status_t pwd_command(command_t *command, client_t *client)
{
    char *response = NULL;

    if (command->argc != 0) {
        fprintf(stderr, "Usage: pwd\n");
        return COMMAND_FAILURE;
    }
    dprintf(client->sockfd, "PWD\r\n");
    response = read_response(client->sockfd);
    if (!response) {
        fprintf(stderr, "Failed to read response from server\n");
        return COMMAND_FAILURE;
    }
    if (strncmp(response, "257", 3) != 0) {
        fprintf(stderr, "Error: %s", response + 4);
        free(response);
        return COMMAND_FAILURE;
    }
    display_current_path(response);
    free(response);
    return COMMAND_SUCCESS;
}
