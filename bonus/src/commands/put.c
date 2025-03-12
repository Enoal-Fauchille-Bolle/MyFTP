/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** PUT Command
*/

#include "myftp.h"

static void put_result(int sockfd, char *filepath)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_bytes;
    FILE *client_stream = fdopen(sockfd, "w");
    FILE *file_stream = fopen(filepath, "r");

    if (client_stream == NULL || file_stream == NULL) {
        perror("fdopen");
        return;
    }
    read_bytes = getline(&line, &len, file_stream);
    while (read_bytes > 0) {
        fwrite(line, 1, read_bytes, client_stream);
        read_bytes = getline(&line, &len, file_stream);
    }
    free(line);
    fclose(client_stream);
    fclose(file_stream);
}

static command_status_t check_put_response(client_t *client, char *filepath)
{
    char *response = read_response(client->sockfd);

    if (!response)
        return COMMAND_FAILURE;
    if (strncmp(response, "150", 3) != 0) {
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    put_result(client->data_socket->sockfd, filepath);
    response = read_response(client->sockfd);
    if (!response)
        return COMMAND_FAILURE;
    if (strncmp(response, "226", 3) != 0) {
        free(response);
        return COMMAND_FAILURE;
    }
    free(response);
    return COMMAND_SUCCESS;
}

static command_status_t execute_put_command(client_t *client, char *filepath)
{
    char *filename = get_filename_from_filepath(filepath);

    if (access(filepath, R_OK) != 0) {
        fprintf(stderr, "Error: Cannot access file %s\n", filepath);
        return COMMAND_FAILURE;
    }
    dprintf(client->sockfd, "STOR %s\r\n", filename);
    if (check_put_response(client, filepath) == COMMAND_FAILURE) {
        fprintf(stderr, "Error: PUT command failed\n");
        destroy_data_socket(client->data_socket);
        client->data_socket = NULL;
        return COMMAND_FAILURE;
    }
    printf("File '%s' uploaded successfully\n", filename);
    destroy_data_socket(client->data_socket);
    client->data_socket = NULL;
    free(filename);
    return COMMAND_SUCCESS;
}

command_status_t put_command(command_t *command, client_t *client)
{
    if (command->argc != 1) {
        fprintf(stderr, "Usage: PUT <filename>\n");
        return COMMAND_FAILURE;
    }
    if (initialize_client_data_socket(client) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    if (execute_put_command(client, command->argv[0]) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    return COMMAND_SUCCESS;
}
