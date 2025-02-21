/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Client Handler File
*/

#include "myftp.h"

static char *resize_buffer(char *buffer, size_t *size)
{
    char *new_buffer = NULL;

    *size *= 2;
    new_buffer = realloc(buffer, *size);
    return new_buffer ? new_buffer : (free(buffer), NULL);
}

static char *read_socket(int sockfd)
{
    size_t size = BUFFER_SIZE;
    size_t total = 0;
    char *buffer = malloc(size);
    int bytes;

    if (!buffer)
        return NULL;
    while (1) {
        bytes = recv(sockfd, buffer + total, BUFFER_SIZE - 1, 0);
        if (bytes == -1)
            break;
        total += bytes;
        buffer[total] = '\0';
        if (bytes < BUFFER_SIZE - 1)
            break;
        buffer = resize_buffer(buffer, &size);
        if (!buffer)
            return NULL;
    }
    return (bytes < 0) ? (free(buffer), NULL) : buffer;
}

int handle_connection(int sockfd)
{
    char *buffer = NULL;

    dprintf(sockfd, "220 Service ready for new user.\r\n");
    while (1) {
        buffer = read_socket(sockfd);
        printf("Received: %s", buffer);
        if (buffer)
            free(buffer);
        else
            break;
    }
    return 0;
}
