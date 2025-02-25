/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Utilities
*/

#include <ctype.h>

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
