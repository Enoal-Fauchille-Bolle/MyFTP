/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Socket File
*/

#include "myftp.h"

struct sockaddr_in init_sockin(char *ip, int port)
{
    struct sockaddr_in addr = {0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        return (struct sockaddr_in){0};
    }
    return addr;
}
