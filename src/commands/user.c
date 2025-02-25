/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

command_status_t user_command(command_t *command, server_t *server)
{
    (void)server;
    printf("Command: %s\n", command->name);
    for (int i = 0; i < command->argc; i++) {
        printf("Arg %d: %s\n", i, command->argv[i]);
    }
    return 0;
}
