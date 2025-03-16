/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Test USER Command
*/

#include <criterion/assert.h>
#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <sys/stat.h>

#include "myftp.h"

static char *test_path = "/tmp";

static connection_t *setup_test_connection(int *read_fd)
{
    connection_t *connection = malloc(sizeof(connection_t));
    int pipe_fds[2];

    if (!connection)
        return NULL;
    if (pipe(pipe_fds) == -1) {
        free(connection);
        return NULL;
    }
    *read_fd = pipe_fds[0];                   // Store read end
    connection->client_sockfd = pipe_fds[1];  // Write end
    connection->server = malloc(sizeof(server_t));
    if (!connection->server) {
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        free(connection);
        return NULL;
    }
    connection->server->path = strdup(test_path);
    connection->working_directory = strdup(test_path);
    connection->logged_in = false;
    connection->user = NULL;
    connection->data_socket = NULL;
    connection->client_addr = malloc(sizeof(struct sockaddr_in));
    connection->stream = fdopen(pipe_fds[1], "w");
    return connection;
}

static void cleanup_test_connection(connection_t *connection, int read_fd)
{
    if (!connection)
        return;
    close(read_fd);
    destroy_connection(connection, false);
    free(connection->server);
    free(connection);
}

Test(command_user, basic_user_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"Anonymous", NULL};
    command_t command = {.name = strdup("USER"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection);

    command_status = user_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer, "331 Please specify the password.\r\n");
    cr_assert_eq(command_status, COMMAND_SUCCESS);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}

Test(command_user, already_logged_in_user_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"Anonymous", NULL};
    command_t command = {.name = strdup("USER"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection);

    connection->logged_in = true;
    command_status = user_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer, "530 Can't change to another user.\r\n");
    cr_assert_eq(command_status, COMMAND_FAILURE);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}

Test(command_user_empty, empty_user_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {NULL};
    command_t command = {.name = strdup("USER"), .argc = 0, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection);

    // Test unauthenticated user with empty username
    command_status = user_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer, "331 Please specify the password.\r\n");
    cr_assert_eq(command_status, COMMAND_SUCCESS);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}
