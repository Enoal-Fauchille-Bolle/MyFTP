/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Main File
*/

#include "myftp.h"

static void help_page(void)
{
    puts("USAGE: ./myftp_client <ip> <port>");
    puts("\tip is the server ip address");
    puts("\tport is the port number on which the server socket listens");
}

static int ip_is_valid(const char *ip)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));

    return result == 1;
}

int main(int ac, char **av)
{
    int port = 0;

    if ((ac >= 2 && strcmp(av[1], "-help") == 0)) {
        help_page();
        return 0;
    }
    if (ac != 3) {
        help_page();
        return 84;
    }
    port = atoi(av[2]);
    if (port <= 0 || port > 65535 || !ip_is_valid(av[1])) {
        help_page();
        return 84;
    }
    if (port == 0)
        return 84;
    client(av[1], port);
    return 0;
}
