/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Utilities
*/

#include "myftp.h"

char *touppercase(char *str)
{
    if (str == NULL) {
        return NULL;
    }
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
    return str;
}

char *trim(char *str)
{
    char *start = str;
    char *end = start + strlen(start) - 1;

    if (!str || *str == '\0')
        return str;
    while (*start && isspace(*start))
        start++;
    if (*start == '\0') {
        *str = '\0';
        return str;
    }
    while (end > start && isspace(*end)) {
        *end = '\0';
        end--;
    }
    if (start != str)
        memmove(str, start, strlen(start) + 1);
    return str;
}

int merge_port(int ports[2])
{
    if (ports == NULL || ports[0] < 0 || ports[0] > 255 || ports[1] < 0 ||
        ports[1] > 255)
        return -1;
    return (ports[0] * 256) + ports[1];
}
