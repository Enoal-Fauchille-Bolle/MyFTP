/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** PASS Command
*/

#include "myftp.h"

/**
 * @brief Gets the password from the command arguments
 *
 * Returns empty string if no arguments provided, otherwise returns first
 * argument.
 *
 * @param command The parsed command structure
 * @return char* The password string (must be freed by caller)
 */
static char *get_password(command_t *command)
{
    if (command->argc == 0) {
        return strdup("");
    }
    return strdup(command->argv[0]);
}

/**
 * @brief Checks login preconditions
 *
 * Verifies that user is not already logged in and that USERNAME was provided.
 *
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if checks pass, COMMAND_FAILURE
 * otherwise
 */
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

/**
 * @brief Processes the login attempt
 *
 * Validates credentials and updates connection state.
 * Only accepts Anonymous/anonymous user with empty password.
 *
 * @param connection The client connection structure
 * @param password The password to validate
 * @return command_status_t COMMAND_SUCCESS if login successful,
 * COMMAND_FAILURE otherwise
 */
static command_status_t process_login(connection_t *connection, char *password)
{
    if ((strcmp(connection->user, "Anonymous") == 0 ||
            strcmp(connection->user, "anonymous") == 0) &&
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

/**
 * @brief Handles the PASS (Password) FTP command
 *
 * Processes password authentication after USER command.
 * Must be preceded by USER command and user must not be already logged in.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if authentication successful,
 * COMMAND_FAILURE otherwise
 */
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
