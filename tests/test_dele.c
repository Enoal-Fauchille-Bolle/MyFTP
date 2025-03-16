/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Test DELE Command
*/

#include <criterion/assert.h>
#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <sys/stat.h>
#include <errno.h>

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

Test(command_dele, basic_dele_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"test_file.txt", NULL};
    command_t command = {.name = strdup("DELE"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection, "Failed to set up test connection");
    connection->logged_in = true;

    // Create a temporary file to test deletion
    char *temp_file_path = "/tmp/test_file.txt";
    FILE *temp_file = fopen(temp_file_path, "w");
    cr_assert_not_null(temp_file, "Failed to create temporary file");
    fprintf(temp_file, "This is a test file.\n");
    fclose(temp_file);

    connection->logged_in = true;
    command_status = dele_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer,
        "250 Requested file action okay.\r\n");
    cr_assert_eq(command_status, COMMAND_SUCCESS);

    free(command.name);
    cleanup_test_connection(connection, read_fd);

    // Check if the file was deleted
    struct stat stat_buffer;
    int exist = stat(temp_file_path, &stat_buffer);
    cr_assert_eq(exist, -1, "File was not deleted");
    cr_assert_eq(errno, ENOENT, "File still exists");
    // Clean up the temporary file if it still exists
    if (exist == 0) {
        remove(temp_file_path);
    }
}

Test(command_dele, unauthenticated_dele_command)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"test_file.txt", NULL};
    command_t command = {.name = strdup("DELE"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection, "Failed to set up test connection");
    connection->logged_in = false;

    command_status = dele_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer,
        "530 Please login with USER and PASS.\r\n");
    cr_assert_eq(command_status, COMMAND_FAILURE);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}

Test(command_dele, delete_nonexistent_file)
{
    connection_t *connection;
    command_status_t command_status;
    char *args[] = {"nonexistent_file.txt", NULL};
    command_t command = {.name = strdup("DELE"), .argc = 1, .argv = args};
    int read_fd;

    connection = setup_test_connection(&read_fd);
    cr_assert_not_null(connection, "Failed to set up test connection");
    connection->logged_in = true;

    command_status = dele_command(&command, connection);
    char buffer[1024] = {0};
    fflush(connection->stream);  // Ensure data is written
    read(read_fd, buffer, sizeof(buffer));
    cr_assert_str_eq(buffer,
        "550 Delete operation failed.\r\n");
    cr_assert_eq(command_status, COMMAND_FAILURE);

    free(command.name);
    cleanup_test_connection(connection, read_fd);
}