/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** LIST Command
*/

#include "myftp.h"

/**
 * @brief Verifies if a path exists
 *
 * @param connection The client connection structure
 * @param path The path to check
 * @return int Returns 1 if path doesn't exist, 0 otherwise
 */
static int check_exists(connection_t *connection, char *path)
{
    if (access(path, F_OK) != 0) {
        dprintf(
            connection->client_sockfd, "550 Failed to list directory.\r\n");
        return 1;
    }
    return 0;
}

static command_status_t goto_path(connection_t *connection, command_t *command)
{
    char *path = command->argv[0] ? command->argv[0] : ".";

    chdir(connection->working_directory);
    if (chdir(path) == -1) {
        dprintf(
            connection->client_sockfd, "550 Failed to list directory.\r\n");
        return COMMAND_FAILURE;
    }
    return COMMAND_SUCCESS;
}

static command_status_t check_directory_access(
    connection_t *connection, char *path)
{
    char *current_directory = NULL;

    chdir(connection->working_directory);
    if (chdir(path) == -1) {
        dprintf(
            connection->client_sockfd, "550 Failed to list directory.\r\n");
        return COMMAND_FAILURE;
    }
    current_directory = getcwd(NULL, 0);
    if (strncmp(current_directory, connection->server->path,
            strlen(connection->server->path)) != 0) {
        chdir(connection->server->path);
        dprintf(
            connection->client_sockfd, "550 Failed to list directory.\r\n");
        free(current_directory);
        return COMMAND_FAILURE;
    }
    chdir(connection->server->path);
    free(current_directory);
    return COMMAND_SUCCESS;
}

static command_status_t list_directory(connection_t *connection)
{
    FILE *fp = NULL;
    char buffer[1024];

    fp = popen("ls -l", "r");
    if (fp == NULL) {
        perror("popen");
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd,
        "150 Here comes the directory listing.\r\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        dprintf(connection->data_socket->client_sockfd, "%s", buffer);
    pclose(fp);
    return COMMAND_SUCCESS;
}

/**
 * @brief Executes the ls -l command and sends output through data socket
 *
 * Changes to working directory, executes ls -l command, and sends the output
 * through the data socket to the client. Returns to server root after
 * execution.
 *
 * @param connection The client connection structure
 * @param command The parsed command structure (unused)
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE on
 * error
 */
static command_status_t execute_ls_command(
    connection_t *connection, command_t *command)
{
    if (goto_path(connection, command) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    if (list_directory(connection) == COMMAND_FAILURE)
        return COMMAND_FAILURE;
    chdir(connection->server->path);
    dprintf(connection->client_sockfd, "226 Directory send OK.\r\n");
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the LIST FTP command
 *
 * Lists files in the current directory.
 * Requires user to be logged in and a data connection to be established
 * through either PASV or PORT command.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE
 * otherwise
 */
command_status_t list_command(command_t *command, connection_t *connection)
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
    if (command->argc >= 1 &&
        (check_exists(connection, command->argv[0]) == 1 ||
            check_directory_access(connection, command->argv[0]) ==
                COMMAND_FAILURE)) {
        return COMMAND_FAILURE;
    }
    return execute_data_socket_command(
        connection, execute_ls_command, command);
}
