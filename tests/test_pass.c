/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Test PASS Command
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

Test(command_pass, basic_pass_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"", NULL};
    command_t command = {.name = strdup("PASS"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection);

    // Test unauthenticated pass with username set
    connection->user = strdup("Anonymous");
    command_status = pass_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer, "230 Login successful.\r\n");
    cr_assert_eq(command_status, COMMAND_SUCCESS);
    cr_assert_eq(connection->logged_in, true);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}

Test(command_pass, no_user_pass_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"password", NULL};
    command_t command = {.name = strdup("PASS"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection);

    command_status = pass_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer, "503 Login with USER first.\r\n");
    cr_assert_eq(command_status, COMMAND_FAILURE);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}

Test(command_pass, already_login_pass_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"password", NULL};
    command_t command = {.name = strdup("PASS"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection);

    // Test authenticated pass
    connection->logged_in = true;
    command_status = pass_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer, "230 Already logged in.\r\n");
    cr_assert_eq(command_status, COMMAND_FAILURE);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}
