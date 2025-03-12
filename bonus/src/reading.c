/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Reading
*/

#include "myftp.h"

char *read_input(void)
{
    char *buffer = NULL;
    size_t len = 0;

    if (getline(&buffer, &len, stdin) == -1) {
        if (errno != EINTR && errno != 0)
            perror("getline");
        return NULL;
    }
    return buffer;
}

char *read_response(int sockfd)
{
    char *buffer = NULL;
    size_t len = 0;

    if (getline(&buffer, &len, fdopen(sockfd, "r")) == -1) {
        if (errno != EINTR && errno != 0)
            perror("getline");
        return NULL;
    }
    return buffer;
}
