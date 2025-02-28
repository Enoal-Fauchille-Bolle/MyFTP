/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Data Socket
*/

#include "myftp.h"

static int listen_socket(int data_socket_sockfd)
{
    if (listen(data_socket_sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        return -1;
    }
    return 0;
}

static int bind_socket(
    int data_socket_sockfd, struct sockaddr_in *data_socket_addr)
{
    socklen_t addr_len = sizeof(*data_socket_addr);

    if (bind(data_socket_sockfd, (struct sockaddr *)data_socket_addr,
            addr_len) == -1) {
        perror("bind");
        return -1;
    }
    return 0;
}

static struct sockaddr_in init_sockin(void)
{
    struct sockaddr_in data_socket_addr = {0};

    data_socket_addr.sin_family = AF_INET;
    data_socket_addr.sin_port = htons(0);
    data_socket_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return data_socket_addr;
}

static int setup_socket_fd(void)
{
    int data_socket_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (data_socket_sockfd == -1) {
        perror("socket");
    }
    return data_socket_sockfd;
}

static int get_final_port(int data_socket_sockfd)
{
    struct sockaddr_in data_socket_addr = {0};
    socklen_t addr_len = sizeof(data_socket_addr);

    getsockname(
        data_socket_sockfd, (struct sockaddr *)&data_socket_addr, &addr_len);
    return ntohs(data_socket_addr.sin_port);
}

data_socket_t setup_data_socket(void)
{
    int data_socket_sockfd = setup_socket_fd();
    struct sockaddr_in data_socket_addr = {0};
    int final_port = 0;

    if (data_socket_sockfd == -1)
        return (data_socket_t){0};
    data_socket_addr = init_sockin();
    if (bind_socket(data_socket_sockfd, &data_socket_addr) == -1)
        return (data_socket_t){0};
    final_port = get_final_port(data_socket_sockfd);
    if (listen_socket(data_socket_sockfd) == -1)
        return (data_socket_t){0};
    return (data_socket_t){data_socket_sockfd, data_socket_addr, final_port};
}
