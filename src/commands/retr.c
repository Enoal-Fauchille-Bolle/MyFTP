/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** RETR Command
*/

#include "myftp.h"

/**
 * @brief Checks if a file exists and is readable
 *
 * Changes to working directory, attempts to open file, and checks permissions.
 * Sends appropriate error messages to client if file cannot be accessed.
 *
 * @param connection The client connection structure
 * @param command The parsed command structure containing filename
 * @return int 0 if file exists and is readable, -1 otherwise
 */
static int check_file_exists(connection_t *connection, command_t *command)
{
    FILE *fp = NULL;

    chdir(connection->working_directory);
    fp = fopen(command->argv[0], "r");
    chdir(connection->server->path);
    if (fp == NULL && access(command->argv[0], R_OK) != 0) {
        dprintf(connection->client_sockfd, "550 Permission denied.\r\n");
        return -1;
    }
    if (fp == NULL) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return -1;
    }
    fclose(fp);
    return 0;
}

/**
 * @brief Executes the file retrieval operation
 *
 * Opens the requested file and sends its contents through the data connection.
 * Changes to working directory for file operations.
 *
 * @param connection The client connection structure
 * @param command The parsed command structure containing filename
 * @return command_status_t COMMAND_SUCCESS if transfer complete,
 * COMMAND_FAILURE on error
 */
static command_status_t execute_retr_command(
    connection_t *connection, command_t *command)
{
    FILE *fp = NULL;
    char buffer[1024];

    dprintf(connection->client_sockfd,
        "150 Opening BINARY mode data connection.\r\n");
    chdir(connection->working_directory);
    fp = fopen(command->argv[0], "r");
    chdir(connection->server->path);
    if (fp == NULL) {
        perror("fopen");
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        dprintf(connection->data_socket->client_sockfd, "%s", buffer);
    dprintf(connection->client_sockfd, "226 Transfer complete.\r\n");
    fclose(fp);
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the RETR (Retrieve) FTP command
 *
 * Downloads a file from the server to the client.
 * Requires user to be logged in, a data connection to be established,
 * and exactly one argument specifying the file to retrieve.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if file retrieved successfully,
 *                         COMMAND_FAILURE otherwise
 */
command_status_t retr_command(command_t *command, connection_t *connection)
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
    if (check_file_exists(connection, command) == -1)
        return COMMAND_FAILURE;
    return execute_data_socket_command(
        connection, execute_retr_command, command);
}
