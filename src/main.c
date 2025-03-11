/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Main File
*/

#include "myftp.h"
#include <signal.h>

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

static void handle_signal(int sig)
{
    (void)sig;
    printf("\nStopping FTP server...\n");
}

static void setup_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
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
    if (port == 0) {
        puts("Error: Invalid port number");
        return 84;
    } else if (check_path(path) == 84) {
        puts("Error: Invalid path");
        return 84;
    }
    setup_signal();
    myftp(port, path);
    return 0;
}
