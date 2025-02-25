/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Connection File
*/

#include "myftp.h"

// Initializes the pollfd array with the listening socket.
static void init_poll_fds(struct pollfd *fds, int sockfd)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        fds[i].fd = -1;
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
}

// Accepts a new client connection and adds it to the pollfd array.
static void accept_new_connection(int sockfd, struct pollfd *fds, int max_fds)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd =
        accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_fd == -1) {
        perror("accept");
        return;
    }
    printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port));
    for (int i = 1; i < max_fds; i++) {
        if (fds[i].fd < 0) {
            fds[i].fd = client_fd;
            fds[i].events = POLLOUT;
            break;
        }
    }
}

// Processes client events: for each ready client, handle the connection.
static void process_client_events(
    struct pollfd *fds, int max_fds, server_t *server)
{
    for (int i = 1; i < max_fds; i++) {
        if (fds[i].fd < 0)
            continue;
        if (fds[i].revents & POLLOUT) {
            handle_connection(fds[i].fd, server);
            close(fds[i].fd);
            fds[i].fd = -1;
        }
    }
}

// Main connection loop.
int connection_loop(server_t *server)
{
    struct pollfd fds[MAX_CLIENTS + 1];
    int ret = 0;

    init_poll_fds(fds, server->sockfd);
    while (1) {
        ret = poll(fds, MAX_CLIENTS + 1, POLL_TIMEOUT);
        if (ret < 0) {
            perror("poll");
            break;
        }
        if (fds[0].revents & POLLIN) {
            accept_new_connection(server->sockfd, fds, MAX_CLIENTS + 1);
        }
        process_client_events(fds, MAX_CLIENTS + 1, server);
    }
    return 0;
}
