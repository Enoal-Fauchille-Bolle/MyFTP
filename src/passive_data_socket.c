/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Passive Data Socket
*/

#include "myftp.h"

/**
 * @brief Initializes a passive data socket structure
 *
 * @return data_socket_t* Pointer to the initialized data socket structure,
 *                        NULL if malloc fails
 */
static data_socket_t *init_passive_data_socket(void)
{
    data_socket_t *data_socket = malloc(sizeof(data_socket_t));

    if (data_socket == NULL) {
        perror("malloc");
        return NULL;
    }
    data_socket->data_socket_mode = PASSIVE;
    data_socket->address = NULL;
    data_socket->client_sockfd = -1;
    return data_socket;
}

/**
 * @brief Sets up a socket to listen for incoming connections
 *
 * @param data_socket_sockfd The socket file descriptor to listen on
 * @return int 0 on success, -1 on failure
 */
static int listen_socket(int data_socket_sockfd)
{
    if (listen(data_socket_sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        return -1;
    }
    return 0;
}

/**
 * @brief Binds a socket to a specific address
 *
 * @param data_socket_sockfd The socket file descriptor to bind
 * @param data_socket_addr Pointer to the address structure to bind to
 * @return int 0 on success, -1 on failure
 */
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

/**
 * @brief Initializes a sockaddr_in structure for passive mode
 *
 * @return struct sockaddr_in Initialized address structure with loopback
 * address and dynamic port
 */
static struct sockaddr_in init_sockin(void)
{
    struct sockaddr_in data_socket_addr = {0};

    data_socket_addr.sin_family = AF_INET;
    data_socket_addr.sin_port = htons(0);
    data_socket_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return data_socket_addr;
}

/**
 * @brief Creates a new socket file descriptor
 *
 * @return int The created socket file descriptor, -1 on failure
 */
static int setup_socket_fd(void)
{
    int data_socket_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (data_socket_sockfd == -1) {
        perror("socket");
    }
    return data_socket_sockfd;
}

/**
 * @brief Gets the actual port number assigned by the system and stores it
 *
 * @param data_socket Pointer to the data socket structure to update
 * @param data_socket_sockfd The socket file descriptor to get the port from
 */
static void set_final_port(data_socket_t *data_socket, int data_socket_sockfd)
{
    struct sockaddr_in data_socket_addr = {0};
    socklen_t addr_len = sizeof(data_socket_addr);

    getsockname(
        data_socket_sockfd, (struct sockaddr *)&data_socket_addr, &addr_len);
    data_socket->port = ntohs(data_socket_addr.sin_port);
}

/**
 * @brief Sets up a complete passive data socket
 *
 * This function creates and initializes a passive data socket, including:
 * - Allocating the data socket structure
 * - Creating the socket file descriptor
 * - Binding to the loopback address
 * - Setting up the listen queue
 * - Getting the assigned port number
 *
 * @return data_socket_t* Pointer to the configured data socket structure,
 *                        NULL if any step fails
 */
data_socket_t *setup_passive_data_socket(void)
{
    data_socket_t *data_socket = init_passive_data_socket();
    int data_socket_sockfd = setup_socket_fd();
    struct sockaddr_in data_socket_addr = {0};

    if (data_socket == NULL || data_socket_sockfd == -1) {
        perror("malloc");
        return NULL;
    }
    if (data_socket_sockfd == -1)
        return NULL;
    data_socket_addr = init_sockin();
    if (bind_socket(data_socket_sockfd, &data_socket_addr) == -1)
        return NULL;
    set_final_port(data_socket, data_socket_sockfd);
    if (listen_socket(data_socket_sockfd) == -1)
        return NULL;
    data_socket->data_sockfd = data_socket_sockfd;
    data_socket->addr = data_socket_addr;
    return data_socket;
}
