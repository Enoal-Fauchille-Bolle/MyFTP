/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Data Socket
*/

#include "myftp.h"

static data_socket_t *init_active_data_socket(void)
{
    data_socket_t *data_socket = malloc(sizeof(data_socket_t));

    if (data_socket == NULL) {
        perror("malloc");
        return NULL;
    }
    data_socket->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (data_socket->sockfd == -1) {
        perror("socket");
        return NULL;
    }
    return data_socket;
}

static struct sockaddr_in init_data_socket_sockin(int *address, int port)
{
    struct sockaddr_in data_socket_addr = {0};

    data_socket_addr.sin_family = AF_INET;
    data_socket_addr.sin_port = htons(port);
    data_socket_addr.sin_addr.s_addr =
        htonl((address[0] << 24) + (address[1] << 16) + (address[2] << 8) +
        address[3]);
    return data_socket_addr;
}

static data_socket_t *setup_data_socket(int *host_port)
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
    data_socket->addr = init_data_socket_sockin(address, port);
    data_socket->address = address;
    data_socket->port = port;
    free(host_port);
    return data_socket;
}

static char *send_pasv_command(client_t *client)
{
    char *response = NULL;

    dprintf(client->sockfd, "PASV\r\n");
    response = read_response(client->sockfd);
    if (!response) {
        fprintf(stderr, "Error reading PASV response\n");
        return NULL;
    }
    if (strncmp(response, "227", 3) != 0) {
        fprintf(stderr, "Error: %s\n", response);
        free(response);
        return NULL;
    }
    return response;
}

static int *parse_pasv_response(char *response)
{
    int *host_port = malloc(sizeof(int) * 6);

    if (!host_port) {
        perror("malloc");
        return NULL;
    }
    sscanf(response, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
        &host_port[0], &host_port[1], &host_port[2], &host_port[3],
        &host_port[4], &host_port[5]);
    return host_port;
}

data_socket_t *create_data_socket(client_t *client)
{
    char *response = NULL;
    int *host_port = NULL;

    response = send_pasv_command(client);
    if (!response)
        return NULL;
    host_port = parse_pasv_response(response);
    free(response);
    if (!host_port)
        return NULL;
    return setup_data_socket(host_port);
}

command_status_t connect_data_socket(data_socket_t *data_socket)
{
    if (connect(data_socket->sockfd, (struct sockaddr *)&data_socket->addr,
            sizeof(data_socket->addr)) == -1) {
        perror("connect");
        return COMMAND_FAILURE;
    }
    return COMMAND_SUCCESS;
}

command_status_t initialize_client_data_socket(client_t *client)
{
    client->data_socket = create_data_socket(client);
    if (!client->data_socket) {
        fprintf(stderr, "Error creating data socket\n");
        return COMMAND_FAILURE;
    }
    if (connect_data_socket(client->data_socket) == COMMAND_FAILURE) {
        return COMMAND_FAILURE;
    }
    return COMMAND_SUCCESS;
}
