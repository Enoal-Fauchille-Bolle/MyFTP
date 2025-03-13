/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Utilities
*/

#include "myftp.h"

/**
 * @brief Converts a string to uppercase in-place
 *
 * @param str The string to convert
 * @return char* The converted string, or NULL if input is NULL
 */
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

/**
 * @brief Merges two port numbers into a single port value
 *
 * Combines two numbers (0-255) into a single port number using the formula:
 * port = (ports[0] * 256) + ports[1]
 *
 * @param ports Array of two integers representing port components
 * @return int The merged port number, or -1 if input is invalid
 */
int merge_port(int ports[2])
{
    if (ports == NULL || ports[0] < 0 || ports[0] > 255 || ports[1] < 0 ||
        ports[1] > 255)
        return -1;
    return (ports[0] * 256) + ports[1];
}

/**
 * @brief Splits a port number into two components
 *
 * Splits a port number (0-65535) into two numbers (0-255) using the formula:
 * ports[0] = port / 256
 * ports[1] = port % 256
 *
 * @param port The port number to split
 * @return int* Array of two integers containing split components,
 *              or NULL if allocation fails or input is invalid
 */
int *split_port(int port)
{
    int *ports = NULL;

    if (port < 0 || port > 65535)
        return NULL;
    ports = malloc(2 * sizeof(int));
    if (ports == NULL)
        return NULL;
    ports[0] = port / 256;
    ports[1] = port % 256;
    return ports;
}

/**
 * @brief Replaces dots with commas in a string in-place
 *
 * Used for converting IP addresses to the format required by FTP protocol
 *
 * @param string The string to modify
 * @return char* The modified string
 */
char *replace_dots_with_commas(char *string)
{
    for (char *current = string; *current; current++) {
        if (*current == '.') {
            *current = ',';
        }
    }
    return string;
}

/**
 * @brief Removes leading and trailing whitespace from a string in-place
 *
 * @param str The string to trim
 * @return char* The trimmed string, or original string if input is NULL or
 * empty
 */
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
