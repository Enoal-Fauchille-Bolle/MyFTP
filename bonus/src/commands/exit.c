/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** EXIT Command
*/

#include "myftp.h"

command_status_t exit_command(command_t *command, client_t *client)
{
    (void)command;
    (void)client;
    return COMMAND_QUIT;
}
