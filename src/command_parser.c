/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Command Parser
*/

#include "myftp.h"

/**
 * @brief Initializes a new command structure
 *
 * Allocates memory for a new command structure and initializes its members to
 * NULL/0.
 *
 * @return Pointer to initialized command structure, NULL if allocation fails
 */
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

/**
 * @brief Counts the number of arguments in a command string
 *
 * Counts space-separated tokens in the input buffer.
 *
 * @param buffer Input string containing the command and its arguments
 * @return Number of space-separated tokens in the buffer
 */
static int count_args(char *buffer)
{
    int count = 0;

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ')
            count++;
    }
    return count + 1;
}

/**
 * @brief Splits the input buffer into tokens
 *
 * Creates an array of strings by splitting the input buffer on spaces.
 * Removes trailing CR/LF from the last token.
 *
 * @param buffer Input string to tokenize
 * @param token_count Expected number of tokens
 * @return Array of token strings, NULL if allocation fails
 */
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

/**
 * @brief Initializes a command structure with parsed tokens
 *
 * Sets up the command structure members using the tokenized input.
 * First token becomes command name, remaining tokens become arguments.
 *
 * @param command Command structure to initialize
 * @param tokens Array of tokenized strings
 * @param token_count Number of tokens in the array
 */
static void setup_command(command_t *command, char **tokens, int token_count)
{
    command->argc = token_count - 1;
    command->name = tokens[0];
    command->argv = tokens + 1;
}

/**
 * @brief Parses an input buffer into a command structure
 *
 * Main parsing function that processes raw input into a structured command.
 * Trims whitespace, tokenizes input, and builds command structure.
 *
 * @param buffer Raw input string to parse
 * @return Pointer to parsed command structure, NULL if parsing fails or input
 * is empty
 */
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
