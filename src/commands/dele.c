/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** DELE Command
*/

#include "myftp.h"

/**
 * @brief Deletes a file from the current working directory
 *
 * Changes to the working directory and attempts to delete the specified file.
 * Sends appropriate success or failure message to the client.
 *
 * @param connection The client connection structure
 * @param filepath The path of the file to delete
 * @return command_status_t COMMAND_SUCCESS if deletion successful,
 * COMMAND_FAILURE otherwise
 */
static command_status_t delete_file(connection_t *connection, char *filepath)
{
    chdir(connection->working_directory);
    if (remove(filepath) != 0) {
        dprintf(connection->client_sockfd, "550 Delete operation failed.\r\n");
        return COMMAND_FAILURE;
    }
    dprintf(connection->client_sockfd, "250 Requested file action okay.\r\n");
    return COMMAND_SUCCESS;
}

/**
 * @brief Handles the DELE (Delete) FTP command
 *
 * Deletes a file on the server.
 * Requires user to be logged in and exactly one argument specifying the file
 * to delete.
 *
 * @param command The parsed command structure
 * @param connection The client connection structure
 * @return command_status_t COMMAND_SUCCESS if successful, COMMAND_FAILURE
 * otherwise
 */
command_status_t dele_command(command_t *command, connection_t *connection)
{
    if (!connection->logged_in) {
        dprintf(connection->client_sockfd,
            "530 Please login with USER and PASS.\r\n");
        return COMMAND_FAILURE;
    }
    if (command->argc != 1) {
        dprintf(connection->client_sockfd, "550 Delete operation failed.\r\n");
        return COMMAND_FAILURE;
    }
    return delete_file(connection, command->argv[0]);
}
