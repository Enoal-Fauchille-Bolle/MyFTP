/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Command Parser
*/

#include "myftp.h"

static int count_args(char *buffer)
{
    int count = 0;

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ')
            count++;
    }
    return count + 1;
}

command_t parse_buffer(char *buffer)
{
    command_t command = {0};
    int token_count = count_args(buffer);
    char **tokens = malloc(sizeof(char *) * (token_count + 1));
    char *token = strtok(buffer, " ");

    if (tokens == NULL) {
        perror("malloc");
        return command;
    }
    command.argc = token_count - 1;
    for (int i = 0; token != NULL; i++) {
        tokens[i] = strdup(token);
        token = strtok(NULL, " ");
    }
    tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] = '\0';
    tokens[token_count] = NULL;
    command.name = tokens[0];
    command.argv = tokens + 1;
    return command;
}
