/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Main Header
*/

#ifndef MYFTP_H_
    #define MYFTP_H_

    #define MAX_CLIENTS 128
    #define BUFFER_SIZE 1024
    #define POLL_TIMEOUT 10000

    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <string.h>

typedef struct server_s {
    int sockfd;
    struct sockaddr_in addr;
} server_t;

int handle_connection(int sockfd);
server_t setup_socket(int port);
int connection_loop(int sockfd);

#endif /* !MYFTP_H_ */
