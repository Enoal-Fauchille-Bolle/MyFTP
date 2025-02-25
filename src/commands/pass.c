/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

static char *get_password(command_t *command)
{
    if (command->argc == 0) {
        return strdup("");
    }
    return strdup(command->argv[0]);
}

static command_status_t check_preconditions(connection_t *connection)
{
    if (connection->logged_in) {
        dprintf(connection->client_sockfd, "230 Already logged in.\r\n");
        return COMMAND_FAILURE;
    }
    if (connection->user == NULL) {
        dprintf(connection->client_sockfd, "503 Login with USER first.\r\n");
        return COMMAND_FAILURE;
    }
    return COMMAND_SUCCESS;
}

static command_status_t process_login(connection_t *connection, char *password)
{
    if (strcmp(connection->user, "anonymous") == 0 &&
        strcmp(password, "") == 0) {
        dprintf(connection->client_sockfd, "230 Login successful.\r\n");
        connection->logged_in = true;
        return COMMAND_SUCCESS;
    }
    free(connection->user);
    connection->user = NULL;
    dprintf(connection->client_sockfd, "530 Login incorrect.\r\n");
    return COMMAND_FAILURE;
}

command_status_t pass_command(command_t *command, connection_t *connection)
{
    command_status_t status = check_preconditions(connection);
    char *password = NULL;

    if (status == COMMAND_FAILURE)
        return status;
    password = get_password(command);
    status = process_login(connection, password);
    free(password);
    return status;
}
