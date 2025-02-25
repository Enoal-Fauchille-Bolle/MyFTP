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
    {"LIST", list_command}, {NULL, NULL}};

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

command_status_t execute_command(command_t *command, connection_t *connection)
{
    command_handler_t handler = get_command_handler(command);

    if (!handler.handler) {
        if (connection->logged_in) {
            dprintf(connection->client_sockfd, "500 Unknown command.\r\n");
        } else {
            dprintf(connection->client_sockfd,
                "530 Please login with USER and PASS.\r\n");
        }
        return COMMAND_NOT_FOUND;
    }
    // printf("Executing command: %s\n", command->name);
    return handler.handler(command, connection);
}
