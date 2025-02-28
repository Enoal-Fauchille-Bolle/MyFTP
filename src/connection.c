/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Connection File
*/

#include <errno.h>

#include "myftp.h"

static connection_t init_connection(
    server_t *server, int client_sockfd, struct sockaddr_in *client_addr)
{
    connection_t connection = {0};

    connection.server = server;
    connection.client_sockfd = client_sockfd;
    connection.client_addr = malloc(sizeof(struct sockaddr_in));
    if (connection.client_addr != NULL)
        memcpy(
            connection.client_addr, client_addr, sizeof(struct sockaddr_in));
    connection.stream = fdopen(client_sockfd, "r");
    if (connection.stream == NULL) {
        perror("fdopen");
        close(client_sockfd);
        connection = (connection_t){0};
        return connection;
    }
    connection.logged_in = false;
    connection.user = NULL;
    connection.working_directory = strdup(server->path);
    connection.data_socket = NULL;
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
    int client_sockfd = accept(server->sockfd,
        (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_sockfd == -1)
        return perror("accept");
    printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port));
    dprintf(client_sockfd, "220 Service ready for new user.\r\n");
    for (int i = 1; i < max_fds; i++) {
        if (fds[i].fd < 0) {
            connections[i] =
                init_connection(server, client_sockfd, &client_addr);
            fds[i].fd = client_sockfd;
            fds[i].events = POLLIN;
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
        if (fds[i].revents & POLLIN) {
            handle_connection(&fds[i], &connections[i]);
        }
    }
}

static int process_connection(
    server_t *server, struct pollfd *fds, connection_t *connections)
{
    int result = poll(fds, MAX_CLIENTS + 1, POLL_TIMEOUT);

    if (result < 0) {
        if (errno != EINTR)
            perror("poll");
        return 1;
    }
    if (fds[0].revents & POLLIN) {
        accept_new_connection(server, connections, fds, MAX_CLIENTS + 1);
    }
    process_client_events(fds, MAX_CLIENTS + 1, connections);
    return 0;
}

// Main connection loop.
int process_connections(server_t *server)
{
    struct pollfd fds[MAX_CLIENTS + 1];
    connection_t connections[MAX_CLIENTS + 1];

    init_poll_fds(fds, server->sockfd);
    while (1) {
        if (process_connection(server, fds, connections))
            break;
    }
    destroy_server(server, fds, connections);
    return 0;
}
