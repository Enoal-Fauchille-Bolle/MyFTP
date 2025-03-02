/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** QUIT Command
*/

#include "myftp.h"

command_status_t quit_command(command_t *command, connection_t *connection)
{
    (void)command;
    (void)connection;
    return COMMAND_QUIT;
}
