/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Command Executor
*/

#include "myftp.h"

const command_handler_t command_handlers[] = {{"USER", user_command},
    {"PASS", pass_command}, {"CWD", cwd_command}, {"CDUP", cdup_command},
    {"QUIT", quit_command}, {"DELE", dele_command}, {"PWD", pwd_command},
    {"PASV", pasv_command}, {"PORT", port_command}, {"HELP", help_command},
    {"NOOP", noop_command}, {"RETR", retr_command}, {"STOR", stor_command},
    {"LIST", list_command}, {"TYPE", type_command}, {NULL, NULL}};

/**
 * @brief Sends appropriate error message for unknown or unauthorized commands
 *
 * Sends different error messages depending on whether the client is logged in:
 * - For logged-in users: "500 Unknown command"
 * - For non-logged-in users: "530 Please login with USER and PASS"
 *
 * @param connection The client connection structure
 */
static void not_found_message(connection_t *connection)
{
    if (connection->logged_in) {
        dprintf(connection->client_sockfd, "500 Unknown command.\r\n");
    } else {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
    }
}

/**
 * @brief Finds the handler function for a given command
 *
 * Converts command name to uppercase and searches for a matching handler
 * in the command_handlers array.
 *
 * @param command The command structure containing the command name
 * @return command_handler_t structure with handler function, or {NULL, NULL}
 * if not found
 */
static command_handler_t get_command_handler(command_t *command)
{
    touppercase(command->name);
    for (int i = 0; command_handlers[i].command_name; i++) {
        if (strcmp(command_handlers[i].command_name, command->name) == 0) {
            return command_handlers[i];
        }
    }
    return (command_handler_t){NULL, NULL};
}

/**
 * @brief Executes a client command
 *
 * Processes the command by finding its handler and executing it.
 * Sends appropriate error messages if command is invalid or not found.
 *
 * @param command The parsed command structure to execute
 * @param connection The client connection structure
 * @return command_status_t indicating the result of command execution
 */
command_status_t execute_command(command_t *command, connection_t *connection)
{
    command_handler_t handler = {0};

    if (!command) {
        not_found_message(connection);
        return COMMAND_NOT_FOUND;
    }
    handler = get_command_handler(command);
    if (!handler.handler) {
        not_found_message(connection);
        return COMMAND_NOT_FOUND;
    }
    return handler.handler(command, connection);
}
