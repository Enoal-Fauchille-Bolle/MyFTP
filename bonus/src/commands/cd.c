/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** CD Command
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
        printf("New directory: %s\n", path);
    } else {
        fprintf(stderr, "Invalid response format: %s\n", response);
    }
}

static void display_new_path(client_t *client)
{
    char *response = read_response(client->sockfd);

    dprintf(client->sockfd, "PWD\r\n");
    if (!response) {
        fprintf(stderr, "Failed to read response from server\n");
    }
    if (strncmp(response, "257", 3) != 0) {
        fprintf(stderr, "Error: %s", response + 4);
        free(response);
    }
    display_current_path(response);
}

command_status_t cd_command(command_t *command, client_t *client)
{
    char *response = NULL;

    if (command->argc != 1) {
        fprintf(stderr, "Usage: cd <directory>\n");
        return COMMAND_FAILURE;
    }
    dprintf(client->sockfd, "CWD %s\r\n", command->argv[0]);
    response = read_response(client->sockfd);
    if (!response) {
        fprintf(stderr, "Failed to read response from server\n");
        return COMMAND_FAILURE;
    }
    if (strncmp(response, "250", 3) != 0) {
        fprintf(stderr, "Error: %s", response + 4);
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    display_new_path(client);
    return COMMAND_SUCCESS;
}
