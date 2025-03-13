/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Main File
*/

#include "myftp.h"

/**
 * @brief Controls read operation on standard input
 *
 * @param a Boolean flag to determine if read operation should be performed
 */
static void set_read(bool a)
{
    if (a == true)
        read(STDIN_FILENO, NULL, 0);
    return;
}

/**
 * @brief Displays the help/usage message for the FTP server
 *
 * Prints information about command-line arguments including the port number
 * and path requirements for running the server.
 */
static void help_page(void)
{
    puts("USAGE: ./myftp port path");
    puts("\tport is the port number on which the server socket listens");
    puts("\tpath is the path to the home directory for the Anonymous user");
}

/**
 * @brief Validates the provided directory path
 *
 * Checks if the given path exists and is accessible.
 *
 * @param path The directory path to validate
 * @return 0 if path is valid, 84 if path is invalid or inaccessible
 */
static int check_path(char *path)
{
    if (path == NULL)
        return 84;
    if (access(path, F_OK) == -1) {
        return 84;
    }
    return 0;
}

/**
 * @brief Validates command line arguments
 *
 * Checks if the port number is within valid range (1-65535) and if the
 * provided path is valid.
 *
 * @param port The port number to validate
 * @param path The directory path to validate
 * @return 0 if arguments are valid, 84 if any argument is invalid
 */
static int check_args(int port, char *path)
{
    if (port <= 0 || port > 65535) {
        puts("Error: Invalid port number");
        return 84;
    } else if (check_path(path) == 84) {
        puts("Error: Invalid path");
        return 84;
    }
    return 0;
}

/**
 * @brief Sets up an FTP server on the specified port and directory
 *
 * This function initializes and runs an FTP server which serves files from the
 * specified directory path. The server listens on the provided port number for
 * incoming connections.
 *
 * @param port The TCP port number on which the FTP server will listen
 * @param path The file system path to the directory that will be served via
 * FTP
 *
 * @return 0 on successful execution, negative value on error
 */
static int myftp(int port, char *path)
{
    server_t server = {0};

    set_read(false);
    server = setup_socket(port, path);
    if (server.sockfd == 0)
        return 84;
    printf("Listening on port %d\n", port);
    printf("Home directory: %s\n", path);
    return process_connections(&server);
}

/**
 * @brief Main entry point of the FTP server application.
 *
 * Initializes the server, handles command-line arguments, sets up socket
 * connections, processes client requests, and manages the server lifecycle.
 *
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit status code (0 for success, non-zero for errors)
 */
int main(int ac, char **av)
{
    int port = 0;
    char *path = NULL;

    if (ac != 3) {
        help_page();
        return 84;
    }
    if ((ac >= 2 && strcmp(av[1], "-help") == 0)) {
        help_page();
        return 0;
    }
    port = atoi(av[1]);
    path = av[2];
    if (check_args(port, path) == 84)
        return 84;
    setup_signal();
    myftp(port, path);
    return 0;
}

// int main(void)
// {
//     command_t *command = parse_buffer("   ");

//     (void)myftp;
//     (void)check_args;
//     (void)help_page;
//     destroy_command(command);
//     return 0;
// }
