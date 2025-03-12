/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** GET Command
*/

#include "myftp.h"

static void get_result(int sockfd, char *filename)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_bytes;
    FILE *client_stream = fdopen(sockfd, "r");
    FILE *file_stream = fopen(filename, "w");

    if (client_stream == NULL || file_stream == NULL) {
        perror("fdopen");
        return;
    }
    read_bytes = getline(&line, &len, client_stream);
    while (read_bytes > 0) {
        fwrite(line, 1, read_bytes, file_stream);
        read_bytes = getline(&line, &len, client_stream);
    }
    free(line);
    fclose(client_stream);
    fclose(file_stream);
}

static command_status_t check_get_response(int sockfd)
{
    char *response = read_response(sockfd);

    if (!response)
        return COMMAND_FAILURE;
    if (strncmp(response, "150", 3) != 0) {
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    response = read_response(sockfd);
    if (!response)
        return COMMAND_FAILURE;
    if (strncmp(response, "226", 3) != 0) {
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    return COMMAND_SUCCESS;
}

static command_status_t execute_get_command(client_t *client, char *filepath)
{
    char *filename = get_filename_from_filepath(filepath);

    if (!filename) {
        fprintf(stderr, "Error: Failed to extract filename from filepath\n");
        return COMMAND_FAILURE;
    }
    dprintf(client->sockfd, "RETR %s\r\n", filepath);
    if (check_get_response(client->sockfd) == COMMAND_FAILURE) {
        fprintf(stderr, "Error: GET command failed\n");
        destroy_data_socket(client->data_socket);
        client->data_socket = NULL;
        return COMMAND_FAILURE;
    }
    get_result(client->data_socket->sockfd, filename);
    printf("File '%s' downloaded successfully\n", filename);
    destroy_data_socket(client->data_socket);
    client->data_socket = NULL;
    free(filename);
    return COMMAND_SUCCESS;
}

command_status_t get_command(command_t *command, client_t *client)
{
    if (command->argc != 1) {
        fprintf(stderr, "Usage: GET <filename>\n");
        return COMMAND_FAILURE;
    }
    if (initialize_client_data_socket(client) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    if (execute_get_command(client, command->argv[0]) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    return COMMAND_SUCCESS;
}
