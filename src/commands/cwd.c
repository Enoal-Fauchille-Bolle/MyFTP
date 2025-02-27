/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

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

static int check_exists(connection_t *connection, char *path)
{
    if (access(path, F_OK) != 0) {
        dprintf(
            connection->client_sockfd, "550 Failed to change directory.\r\n");
        return 1;
    }
    return 0;
}

static void check_new_path(connection_t *connection)
{
    if (strncmp(connection->working_directory, connection->server->path,
            strlen(connection->server->path)) != 0) {
        free(connection->working_directory);
        connection->working_directory = getcwd(NULL, 0);
    }
}

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

command_status_t cwd_command(command_t *command, connection_t *connection)
{
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(
            connection->client_sockfd, "550 Failed to change directory.\r\n");
        return COMMAND_FAILURE;
    }
    return change_dir(connection, command->argv[0]);
}
