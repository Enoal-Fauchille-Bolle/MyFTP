/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** CDUP Command
*/

#include "myftp.h"

/**
 * @brief Checks if a directory is valid and closes it
 *
 * @param connection The client connection structure
 * @param directory The directory to check
 * @return int Returns 1 if directory is invalid, 0 otherwise
 */
static int check_directory(connection_t *connection, DIR *directory)
{
    if (directory != NULL) {
        closedir(directory);
    } else {
        dprintf(
            connection->client_sockfd, "550 Failed to change directory.\r\n");
        return 1;
    }
    return 0;
}

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
            connection->client_sockfd, "550 Failed to change directory.\r\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Validates and updates the working directory path
 *
 * @param connection The client connection structure
 */
static void check_new_path(connection_t *connection)
{
    if (strncmp(connection->working_directory, connection->server->path,
            strlen(connection->server->path)) != 0) {
        free(connection->working_directory);
        connection->working_directory = getcwd(NULL, 0);
    }
}

/**
 * @brief Changes the current working directory
 *
 * @param connection The client connection structure
 * @param path The target directory path
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE
 * otherwise
 */
static command_status_t change_dir(connection_t *connection, char *path)
{
    DIR *directory = NULL;

    if (check_exists(connection, path) == 1)
        return COMMAND_FAILURE;
    directory = opendir(path);
    if (check_directory(connection, directory) == 1)
        return COMMAND_FAILURE;
    chdir(connection->working_directory);
    chdir(path);
    free(connection->working_directory);
    connection->working_directory = getcwd(NULL, 0);
    chdir(connection->server->path);
    check_new_path(connection);
    dprintf(
        connection->client_sockfd, "250 Directory successfully changed.\r\n");
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the CDUP (Change Directory Up) FTP command
 *
 * Changes the current working directory to the parent directory.
 * Requires user to be logged in.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE
 * otherwise
 */
command_status_t cdup_command(command_t *command, connection_t *connection)
{
    (void)command;
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    return change_dir(connection, "..");
}
