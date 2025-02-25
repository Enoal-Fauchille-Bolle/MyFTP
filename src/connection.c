/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Connection File
*/

#include "myftp.h"

static connection_t init_connection(
    server_t *server, int client_sockfd, struct sockaddr_in *client_addr)
{
    connection_t connection = {0};

    connection.server = server;
    connection.client_sockfd = client_sockfd;
    connection.client_addr = client_addr;
    connection.logged_in = false;
    connection.user = NULL;
    connection.working_directory = strdup(server->path);
    return connection;
}

// Initializes the pollfd array with the listening socket.
static void init_poll_fds(struct pollfd *fds, int server_sockfd)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        fds[i].fd = -1;
    }
    fds[0].fd = server_sockfd;
    fds[0].events = POLLIN;
}

// Accepts a new client connection and adds it to the pollfd array.
static void accept_new_connection(server_t *server, connection_t *connections,
    struct pollfd *fds, int max_fds)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server->server_sockfd,
        (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_fd == -1)
        return perror("accept");
    printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port));
    for (int i = 1; i < max_fds; i++) {
        if (fds[i].fd < 0) {
            connections[i] = init_connection(server, client_fd, &client_addr);
            fds[i].fd = client_fd;
            fds[i].events = POLLOUT;
            break;
        }
    }
}

// Processes client events: for each ready client, handle the connection.
static void process_client_events(
    struct pollfd *fds, int max_fds, connection_t *connections)
{
    for (int i = 1; i < max_fds; i++) {
        if (fds[i].fd < 0)
            continue;
        if (fds[i].revents & POLLOUT) {
            handle_connection(fds[i].fd, &connections[i]);
            close(fds[i].fd);
            fds[i].fd = -1;
        }
    }
}

// Main connection loop.
int process_connections(server_t *server)
{
    struct pollfd fds[MAX_CLIENTS + 1];
    connection_t connections[MAX_CLIENTS + 1];
    int ret = 0;

    init_poll_fds(fds, server->server_sockfd);
    while (1) {
        ret = poll(fds, MAX_CLIENTS + 1, POLL_TIMEOUT);
        if (ret < 0) {
            perror("poll");
            break;
        }
        if (fds[0].revents & POLLIN) {
            accept_new_connection(server, connections, fds, MAX_CLIENTS + 1);
        }
        process_client_events(fds, MAX_CLIENTS + 1, connections);
    }
    return 0;
}
