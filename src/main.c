/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Main File
*/

#include "myftp.h"

static void help_page(void)
{
    puts("USAGE: ./myftp port path");
    puts("\tport is the port number on which the server socket listens");
    puts("\tpath is the path to the home directory for the Anonymous user");
}

static int check_path(char *path)
{
    if (path == NULL)
        return 84;
    if (access(path, F_OK) == -1) {
        return 84;
    }
    return 0;
}

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

static int myftp(int port, char *path)
{
    server_t server = {0};

    server = setup_socket(port, path);
    if (server.sockfd == 0)
        return 84;
    printf("Listening on port %d\n", port);
    printf("Home directory: %s\n", path);
    return process_connections(&server);
}

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
