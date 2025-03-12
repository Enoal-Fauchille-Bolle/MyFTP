/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Command Parser
*/

#include "myftp.h"

static command_t *init_command(void)
{
    command_t *command = malloc(sizeof(command_t));

    if (command == NULL) {
        perror("malloc");
        return NULL;
    }
    command->name = NULL;
    command->argc = 0;
    command->argv = NULL;
    return command;
}

static int count_args(char *buffer)
{
    int count = 0;

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ')
            count++;
    }
    return count + 1;
}

static char **tokenize_buffer(char *buffer, int token_count)
{
    char **tokens = malloc(sizeof(char *) * (token_count + 1));
    char *token = NULL;

    if (tokens == NULL) {
        perror("malloc");
        return NULL;
    }
    token = strtok(buffer, " ");
    for (int i = 0; token != NULL; i++) {
        tokens[i] = strdup(token);
        token = strtok(NULL, " ");
    }
    if (tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] == '\r')
        tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] = '\0';
    if (tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] == '\n')
        tokens[token_count - 1][strlen(tokens[token_count - 1]) - 1] = '\0';
    tokens[token_count] = NULL;
    return tokens;
}

static void setup_command(command_t *command, char **tokens, int token_count)
{
    command->argc = token_count - 1;
    command->name = tokens[0];
    command->argv = tokens + 1;
}

command_t *parse_buffer(char *buffer)
{
    command_t *command = init_command();
    int token_count = 0;
    char **tokens = NULL;

    if (command == NULL)
        return NULL;
    trim(buffer);
    if (strlen(buffer) == 0) {
        free(command);
        return NULL;
    }
    token_count = count_args(buffer);
    tokens = tokenize_buffer(buffer, token_count);
    if (tokens == NULL) {
        free(command);
        return NULL;
    }
    setup_command(command, tokens, token_count);
    return command;
}
