/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Command Executor
*/

#include "myftp.h"

const command_handler_t command_handlers[] = {{"QUIT", quit_command},
    {"EXIT", exit_command}, {"HELP", help_command}, {"LS", ls_command},
    {"CD", cd_command}, {"PWD", pwd_command}, {"GET", get_command},
    {"PUT", put_command}, {NULL, NULL}};

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

command_status_t execute_command(command_t *command, client_t *client)
{
    command_handler_t handler = {0};

    if (!command) {
        printf("Unknown command\r\n");
        return COMMAND_NOT_FOUND;
    }
    handler = get_command_handler(command);
    if (!handler.handler) {
        printf("Unknown command\r\n");
        return COMMAND_NOT_FOUND;
    }
    return handler.handler(command, client);
}
