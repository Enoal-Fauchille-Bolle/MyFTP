/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Socket File
*/

#include "myftp.h"

static int listen_socket(int sockfd)
{
    if (listen(sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        return -1;
    }
    return 0;
}

static int bind_socket(int sockfd, struct sockaddr_in *addr)
{
    socklen_t addr_len = sizeof(*addr);

    if (bind(sockfd, (struct sockaddr *)addr, addr_len) == -1) {
        perror("bind");
        return -1;
    }
    return 0;
}

static struct sockaddr_in init_sockin(int port)
{
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return addr;
}

static int setup_socket_fd(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("socket");
    }
    return sockfd;
}

server_t setup_socket(int port, char *path)
{
    int sockfd = setup_socket_fd();
    struct sockaddr_in addr = {0};

    if (sockfd == -1)
        return (server_t){0};
    addr = init_sockin(port);
    if (bind_socket(sockfd, &addr) == -1)
        return (server_t){0};
    if (listen_socket(sockfd) == -1)
        return (server_t){0};
    return (server_t){sockfd, addr, port, path};
}
