/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** USER Command
*/

#include "myftp.h"

command_status_t help_command(command_t *command, connection_t *connection)
{
    (void)command;
    dprintf(connection->client_sockfd,
        "214-The following commands are recognized.\r\n");
    dprintf(connection->client_sockfd,
        "USER PASS CWD CDUP QUIT DELE PWD PASV PORT HELP NOOP RETR STOR "
        "LIST\r\n");
    dprintf(connection->client_sockfd, "214 Help OK.\r\n");
    return COMMAND_SUCCESS;
}
