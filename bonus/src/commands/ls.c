/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** LS Command
*/

#include "myftp.h"

static void display_result(int sockfd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_bytes;
    FILE *client_stream = fdopen(sockfd, "r");

    if (client_stream == NULL) {
        perror("fdopen");
        return;
    }
    read_bytes = getline(&line, &len, client_stream);
    while (read_bytes > 0) {
        printf("%s", line);
        read_bytes = getline(&line, &len, client_stream);
    }
    free(line);
    fclose(client_stream);
}

static command_status_t check_list_response(int sockfd)
{
    char *response = read_response(sockfd);

    if (!response) {
        return COMMAND_FAILURE;
    }
    if (strncmp(response, "150", 3) != 0) {
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    response = read_response(sockfd);
    if (!response) {
        return COMMAND_FAILURE;
    }
    if (strncmp(response, "226", 3) != 0) {
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    return COMMAND_SUCCESS;
}

static command_status_t execute_ls_command(client_t *client)
{
    dprintf(client->sockfd, "LIST\r\n");
    if (check_list_response(client->sockfd) == COMMAND_FAILURE) {
        fprintf(stderr, "Error: LIST command failed\n");
        destroy_data_socket(client->data_socket);
        client->data_socket = NULL;
        return COMMAND_FAILURE;
    }
    display_result(client->data_socket->sockfd);
    destroy_data_socket(client->data_socket);
    client->data_socket = NULL;
    return COMMAND_SUCCESS;
}

command_status_t ls_command(command_t *command, client_t *client)
{
    (void)command;
    if (initialize_client_data_socket(client) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    if (execute_ls_command(client) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    return COMMAND_SUCCESS;
}
