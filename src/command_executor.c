/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Command Executor
*/

#include "myftp.h"

const command_handler_t command_handlers[] = {
    {"USER", user_command},
    {"QUIT", quit_command},
    {NULL, NULL}
};

command_handler_t get_command_handler(command_t *command)
{
    touppercase(command->name);
    for (int i = 0; command_handlers[i].command_name; i++) {
        if (strcmp(command_handlers[i].command_name,
            command->name) == 0) {
            return command_handlers[i];
        }
    }
    return (command_handler_t){NULL, NULL};
}

command_status_t execute_command(command_t *command, server_t *server)
{
    command_handler_t handler = get_command_handler(command);

    if (!handler.handler) {
        dprintf(2, "Unknown command: '%s'\n", command->name);
        return COMMAND_NOT_FOUND;
    }
    return handler.handler(command, server);
}
