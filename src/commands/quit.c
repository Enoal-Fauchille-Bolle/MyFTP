/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** QUIT Command
*/

#include "myftp.h"

/**
 * @brief Handles the QUIT FTP command
 *
 * Terminates the FTP session and closes the connection.
 * No parameters required, always succeeds.
 *
 * @param command The parsed command structure (unused)
 * @param connection The client connection structure (unused)
 * @return command_status_t Always returns COMMAND_QUIT to signal session
 * termination
 */
command_status_t quit_command(command_t *command, connection_t *connection)
{
    (void)command;
    (void)connection;
    return COMMAND_QUIT;
}
