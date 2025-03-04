/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Active Data Socket
*/

#include "myftp.h"

static data_socket_t *init_active_data_socket(void)
{
    data_socket_t *data_socket = malloc(sizeof(data_socket_t));

    if (data_socket == NULL) {
        perror("malloc");
        return NULL;
    }
    data_socket->mode = ACTIVE;
    data_socket->client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (data_socket->client_sockfd == -1) {
        perror("socket");
        return NULL;
    }
    data_socket->data_sockfd = -1;
    return data_socket;
}

static struct sockaddr_in init_sockin(int *address, int port)
{
    struct sockaddr_in data_socket_addr = {0};

    data_socket_addr.sin_family = AF_INET;
    data_socket_addr.sin_port = htons(port);
    data_socket_addr.sin_addr.s_addr =
        htonl((address[0] << 24) + (address[1] << 16) + (address[2] << 8) +
        address[3]);
    return data_socket_addr;
}

data_socket_t *setup_active_data_socket(int *host_port)
{
    data_socket_t *data_socket = init_active_data_socket();
    int *address = malloc(sizeof(int) * 4);
    int port = merge_port(host_port + 4);

    if (data_socket == NULL || address == NULL) {
        perror("malloc");
        return NULL;
    }
    for (int i = 0; i < 4; i++)
        address[i] = host_port[i];
    data_socket->addr = init_sockin(address, port);
    data_socket->address = address;
    data_socket->port = port;
    return data_socket;
}
