/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Socket File
*/

#include "myftp.h"

/**
 * @brief Sets up a socket to listen for incoming connections
 *
 * @param server_sockfd The socket file descriptor to listen on
 * @return int 0 on success, -1 on failure
 */
static int listen_socket(int server_sockfd)
{
    if (listen(server_sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        return -1;
    }
    return 0;
}

/**
 * @brief Binds a socket to a specific address
 *
 * @param server_sockfd The socket file descriptor to bind
 * @param server_addr Pointer to the address structure to bind to
 * @return int 0 on success, -1 on failure
 */
static int bind_socket(int server_sockfd, struct sockaddr_in *server_addr)
{
    socklen_t addr_len = sizeof(*server_addr);

    if (bind(server_sockfd, (struct sockaddr *)server_addr, addr_len) == -1) {
        perror("bind");
        return -1;
    }
    return 0;
}

/**
 * @brief Initializes a sockaddr_in structure for the server
 *
 * @param port Port number to bind the server to
 * @return struct sockaddr_in Initialized address structure with loopback
 * address and specified port
 */
static struct sockaddr_in init_sockin(int port)
{
    struct sockaddr_in server_addr = {0};

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return server_addr;
}

/**
 * @brief Creates a new socket file descriptor
 *
 * @return int The created socket file descriptor, -1 on failure
 */
static int setup_socket_fd(void)
{
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sockfd == -1) {
        perror("socket");
    }
    return server_sockfd;
}

/**
 * @brief Gets the actual port number assigned by the system
 *
 * @param server_sockfd The socket file descriptor to get the port from
 * @return int The actual port number assigned to the socket
 */
static int set_final_port(int server_sockfd)
{
    struct sockaddr_in server_addr = {0};
    socklen_t addr_len = sizeof(server_addr);

    getsockname(server_sockfd, (struct sockaddr *)&server_addr, &addr_len);
    return ntohs(server_addr.sin_port);
}

/**
 * @brief Sets up a complete server socket
 *
 * This function creates and initializes a server socket, including:
 * - Creating the socket file descriptor
 * - Binding to the specified port
 * - Setting up the listen queue
 * - Changing to the specified directory
 * - Getting the actual working directory path
 *
 * @param port Port number to bind the server to
 * @param raw_path Path to the directory to use as the server's root
 * @return server_t Initialized server structure containing the socket,
 *                  address, port and path information.
 *                  Returns a zeroed structure on any failure.
 */
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
    final_port = set_final_port(server_sockfd);
    if (listen_socket(server_sockfd) == -1)
        return (server_t){0};
    chdir(raw_path);
    path = getcwd(NULL, 0);
    if (path == NULL)
        return (server_t){0};
    return (server_t){server_sockfd, server_addr, final_port, path};
}
