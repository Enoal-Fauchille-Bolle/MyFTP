/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Data Socket
*/

#include "myftp.h"

//////////////////////// Data Socket Command Execution ///////////////////////
static int accept_connection(data_socket_t *data_socket)
{
    socklen_t addr_len = sizeof(data_socket->addr);

    data_socket->client_sockfd = accept(data_socket->data_sockfd,
        (struct sockaddr *)&data_socket->addr, &addr_len);
    if (data_socket->client_sockfd == -1) {
        perror("accept");
        return -1;
    }
    printf("Data connection accepted from %s:%d\n",
        inet_ntoa(data_socket->addr.sin_addr),
        ntohs(data_socket->addr.sin_port));
    return 0;
}

command_status_t execute_data_socket_command(
    connection_t *connection, command_status_t (*command)(connection_t *))
{
    if (accept_connection(connection->data_socket) == -1) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    if (command(connection) == COMMAND_FAILURE) {
        dprintf(connection->client_sockfd,
            "451 Requested action aborted: local error in processing.\r\n");
        return COMMAND_FAILURE;
    }
    close(connection->data_socket->client_sockfd);
    close(connection->data_socket->data_sockfd);
    printf(
        "Data connection closed on port %d\n", connection->data_socket->port);
    free(connection->data_socket);
    connection->data_socket = NULL;
    return COMMAND_SUCCESS;
}

//////////////////////////// Data Socket Creation ////////////////////////////
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
    return (data_socket_t){
        data_socket_sockfd, data_socket_addr, final_port, 0};
}
