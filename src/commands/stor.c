/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** STOR Command
*/

#include "myftp.h"

/**
 * @brief Transfers file data from client to server
 *
 * Reads data from the client's data socket and writes it to the specified
 * file. Uses line-by-line transfer with dynamic buffer allocation.
 *
 * @param file The destination file handle
 * @param data_socket_sockfd The data socket file descriptor
 * @return int 0 on success, -1 on error
 */
static int transfer_file_data(FILE *file, int data_socket_sockfd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read_bytes;
    FILE *client_stream = fdopen(data_socket_sockfd, "r");

    if (client_stream == NULL)
        return -1;
    read_bytes = getline(&line, &len, client_stream);
    while (read_bytes > 0) {
        fwrite(line, 1, read_bytes, file);
        read_bytes = getline(&line, &len, client_stream);
    }
    free(line);
    fclose(client_stream);
    return read_bytes;
}

/**
 * @brief Executes the file storage operation
 *
 * Opens the destination file and transfers data from client connection.
 * Sends appropriate status messages to client.
 *
 * @param connection The client connection structure
 * @param command The parsed command structure containing filename
 * @return command_status_t COMMAND_SUCCESS if transfer complete,
 * COMMAND_FAILURE on error
 */
static command_status_t execute_stor_command(
    connection_t *connection, command_t *command)
{
    FILE *file = fopen(command->argv[0], "wb");

    dprintf(connection->client_sockfd,
        "150 Opening BINARY mode data connection.\r\n");
    if (file == NULL) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    transfer_file_data(file, connection->data_socket->client_sockfd);
    dprintf(connection->client_sockfd, "226 Transfer complete.\r\n");
    fclose(file);
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the STOR (Store) FTP command
 *
 * Uploads a file from the client to the server.
 * Requires user to be logged in, a data connection to be established,
 * and exactly one argument specifying the destination filename.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if file stored successfully,
 * COMMAND_FAILURE otherwise
 */
command_status_t stor_command(command_t *command, connection_t *connection)
{
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (connection->data_socket == NULL) {
        dprintf(connection->client_sockfd, "425 Use PASV or PORT first.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(connection->client_sockfd, "550 Failed to open file.\r\n");
        return COMMAND_FAILURE;
    }
    return execute_data_socket_command(
        connection, execute_stor_command, command);
}
