/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Connection File
*/

#include <errno.h>

#include "myftp.h"

/**
 * @brief Initializes a new client connection structure
 *
 * Creates and initializes a connection structure for a new client,
 * setting up the socket, stream, and default values.
 *
 * @param server Pointer to the server structure
 * @param client_sockfd Client socket file descriptor
 * @param client_addr Client address structure
 * @return Initialized connection structure, zeroed structure on error
 */
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
        return (connection_t){0};
    }
    connection.logged_in = false;
    connection.user = NULL;
    connection.working_directory = strdup(server->path);
    connection.data_socket = NULL;
    connection.transfer_mode = BINARY;
    return connection;
}

/**
 * @brief Initializes poll file descriptors array
 *
 * Sets up the pollfd array for monitoring client connections.
 * First slot is reserved for server socket, rest are initialized to -1.
 *
 * @param fds Array of pollfd structures to initialize
 * @param server_sockfd Server socket file descriptor
 */
static void init_poll_fds(struct pollfd *fds, int server_sockfd)
{
    for (int i = 0; i < MAX_CLIENTS + 1; i++) {
        fds[i].fd = -1;
    }
    fds[0].fd = server_sockfd;
    fds[0].events = POLLIN;
}

/**
 * @brief Accepts and initializes a new client connection
 *
 * Accepts incoming connection, sends welcome message, and adds client
 * to the first available slot in the connections array.
 *
 * @param server Pointer to server structure
 * @param connections Array of client connections
 * @param fds Array of poll file descriptors
 * @param max_fds Maximum number of file descriptors
 */
static void accept_new_connection(server_t *server, connection_t *connections,
    struct pollfd *fds, int max_fds)
{
    struct sockaddr_in client_addr = {0};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(
        server->sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_sockfd == -1)
        return perror("accept");
    printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port));
    dprintf(client_sockfd, "220 Service ready for new user. (myftp)\r\n");
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

/**
 * @brief Processes events for all connected clients
 *
 * Iterates through all client connections and handles any pending events.
 *
 * @param fds Array of poll file descriptors
 * @param max_fds Maximum number of file descriptors
 * @param connections Array of client connections
 */
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

/**
 * @brief Processes a single connection cycle
 *
 * Polls for events and handles new connections and client events.
 *
 * @param server Pointer to server structure
 * @param fds Array of poll file descriptors
 * @param connections Array of client connections
 * @return 0 on success, 1 on error or interrupt
 */
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

/**
 * @brief Main connection processing loop
 *
 * Initializes connection handling structures and runs the main server loop
 * until server is stopped. Handles client connections and cleanup.
 *
 * @param server Pointer to server structure
 * @return 0 on successful completion
 */
int process_connections(server_t *server)
{
    struct pollfd fds[MAX_CLIENTS + 1];
    connection_t connections[MAX_CLIENTS + 1];

    init_poll_fds(fds, server->sockfd);
    while (!is_server_stopped()) {
        if (process_connection(server, fds, connections))
            break;
    }
    destroy_server(server, fds, connections);
    return 0;
}
