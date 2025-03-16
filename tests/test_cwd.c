/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Test CWD Command
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

// Test(command_cwd, basic_cwd_command)
// {
//     connection_t *connection;
//     command_status_t command_status;
//     char *args[] = {"test_dir", NULL};
//     command_t command = {.name = strdup("CWD"), .argc = 1, .argv = args};
//     int read_fd;

//     connection = setup_test_connection(&read_fd);
//     cr_assert_not_null(connection);

//     // Create the test_dir directory
//     if (mkdir("/tmp/test_dir", 0777) == -1) {
//         perror("mkdir");
//     }

//     // Test authenticated cwd
//     connection->logged_in = true;
//     command_status = cwd_command(&command, connection);
//     char buffer[1024] = {0};
//     fflush(connection->stream);  // Ensure data is written
//     read(read_fd, buffer, sizeof(buffer));
//     cr_assert_str_eq(buffer,
//         "250 Directory successfully changed to /tmp/test_dir\r\n");
//     cr_assert_eq(command_status, COMMAND_SUCCESS);

//     // Delete the test_dir directory
//     if (rmdir("/tmp/test_dir") == -1) {
//         perror("rmdir");
//     }

//     free(command.name);
//     cleanup_test_connection(connection, read_fd);
// }

Test(command_cwd, unauthenticated_cwd_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"test_dir", NULL};
    command_t command = {.name = strdup("CWD"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection);

    // Test unauthenticated cwd
    command_status = cwd_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer, "530 Please login with USER and PASS.\r\n");
    cr_assert_eq(command_status, COMMAND_FAILURE);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}
