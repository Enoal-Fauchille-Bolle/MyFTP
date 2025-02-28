/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Socket File
*/

#include "myftp.h"

static int listen_socket(int server_sockfd)
{
    if (listen(server_sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        return -1;
    }
    return 0;
}

static int bind_socket(int server_sockfd, struct sockaddr_in *server_addr)
{
    socklen_t addr_len = sizeof(*server_addr);

    if (bind(server_sockfd, (struct sockaddr *)server_addr, addr_len) == -1) {
        perror("bind");
        return -1;
    }
    return 0;
}

static struct sockaddr_in init_sockin(int port)
{
    struct sockaddr_in server_addr = {0};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return server_addr;
}

static int setup_socket_fd(void)
{
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sockfd == -1) {
        perror("socket");
    }
    return server_sockfd;
}

static int get_final_port(int server_sockfd)
{
    struct sockaddr_in server_addr = {0};
    socklen_t addr_len = sizeof(server_addr);

    getsockname(server_sockfd, (struct sockaddr *)&server_addr, &addr_len);
    return ntohs(server_addr.sin_port);
}

server_t setup_socket(int port, char *raw_path)
{
    int server_sockfd = setup_socket_fd();
    struct sockaddr_in server_addr = {0};
    char *path = NULL;
    int final_port = 0;

    if (server_sockfd == -1)
        return (server_t){0};
    server_addr = init_sockin(port);
    if (bind_socket(server_sockfd, &server_addr) == -1)
        return (server_t){0};
    final_port = get_final_port(server_sockfd);
    if (listen_socket(server_sockfd) == -1)
        return (server_t){0};
    chdir(raw_path);
    path = getcwd(NULL, 0);
    if (path == NULL)
        return (server_t){0};
    return (server_t){server_sockfd, server_addr, final_port, path};
}
