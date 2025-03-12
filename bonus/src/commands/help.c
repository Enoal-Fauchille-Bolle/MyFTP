/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** HELP Command
*/

#include "myftp.h"

command_status_t help_command(command_t *command, client_t *client)
{
    (void)command;
    (void)client;
    printf("Available commands:\n");
    printf("HELP QUIT EXIT LS PWD CD GET PUT\n");
    return COMMAND_SUCCESS;
}
