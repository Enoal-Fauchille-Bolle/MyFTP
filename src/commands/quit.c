/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

command_status_t quit_command(command_t *command, server_t *server)
{
    (void)command;
    (void)server;
    return COMMAND_QUIT;
}
