/*
** EPITECH PROJECT, 2025
** MyFTP Client
** File description:
** Login
*/

#include "myftp.h"

static char *get_username(void)
{
    char *username = NULL;

    printf("Username: ");
    username = read_input();
    if (!username) {
        if (errno != EINTR && errno != 0)
            fprintf(stderr, "Error reading username\n");
    }
    return username;
}

static char *get_password(void)
{
    char *password = NULL;

    printf("Password: ");
    password = read_input();
    if (!password) {
        if (errno != EINTR && errno != 0)
            fprintf(stderr, "Error reading password\n");
    }
    return password;
}

static bool send_user_command(client_t *client, char *username)
{
    char *response = NULL;

    dprintf(client->sockfd, "USER %s", username);
    response = read_response(client->sockfd);
    if (!response || strncmp(response, "331", 3) != 0) {
        fprintf(stderr, "Error sending username: %s\n",
            response ? response : "No response");
        free(response);
        return false;
    }
    free(response);
    return true;
}

static bool send_pass_command(client_t *client, char *password)
{
    char *response = NULL;
    bool success = false;

    dprintf(client->sockfd, "PASS %s", password);
    response = read_response(client->sockfd);
    if (!response) {
        fprintf(stderr, "No response from server\n");
        return false;
    }
    if (strncmp(response, "230", 3) == 0) {
        printf("Login successful\n");
        success = true;
    } else {
        fprintf(stderr, "Login failed: %s", response + 4);
    }
    free(response);
    return success;
}

static int attempt_login(client_t *client)
{
    char *username = NULL;
    char *password = NULL;
    bool logged_in = false;

    username = get_username();
    if (!username)
        return 84;
    if (!send_user_command(client, username)) {
        free(username);
        return 84;
    }
    password = get_password();
    if (!password) {
        free(username);
        return 84;
    }
    logged_in = send_pass_command(client, password);
    free(username);
    free(password);
    return logged_in;
}

int login(client_t *client)
{
    int logged_in = 0;

    free(read_response(client->sockfd));
    while (logged_in == 0 && !is_client_stopped())
        logged_in = attempt_login(client);
    return logged_in;
}
