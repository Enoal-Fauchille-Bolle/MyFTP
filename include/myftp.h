/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Main Header
*/

#ifndef MYFTP_H_
    #define MYFTP_H_

    #define MAX_CLIENTS 128
    #define BUFFER_SIZE 1024
    #define POLL_TIMEOUT 10000

    #include <arpa/inet.h>
    #include <ctype.h>
    #include <dirent.h>
    #include <netinet/in.h>
    #include <poll.h>
    #include <signal.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/socket.h>
    #include <unistd.h>

/**
 * @brief Server configuration structure
 */
typedef struct server_s {
    int sockfd;              /**< Server socket file descriptor */
    struct sockaddr_in addr; /**< Server address information */
    int port;                /**< Server port number */
    char *path;              /**< Server root directory path */
} server_t;

/**
 * @brief Data socket mode enumeration
 */
typedef enum data_socket_mode_e {
    PASSIVE, /**< Passive mode (PASV) */
    ACTIVE,  /**< Active mode (PORT) */
} data_socket_mode_t;

/**
 * @brief Transfer mode enumeration
 */
typedef enum transfer_mode_e {
    BINARY, /**< Binary transfer mode */
    ASCII,  /**< ASCII transfer mode */
} transfer_mode_t;

/**
 * @brief Data socket configuration structure
 */
typedef struct data_socket_s {
    data_socket_mode_t data_socket_mode; /**< Current data socket mode */
    int data_sockfd;         /**< Data socket file descriptor */
    struct sockaddr_in addr; /**< Socket address information */
    int *address;            /**< Remote address for PORT mode */
    int port;                /**< Port number */
    int client_sockfd;       /**< Client socket file descriptor */
} data_socket_t;

/**
 * @brief Client connection structure
 */
typedef struct connection_s {
    server_t *server;                /**< Server configuration */
    int client_sockfd;               /**< Client socket file descriptor */
    struct sockaddr_in *client_addr; /**< Client address information */
    FILE *stream;                    /**< Socket stream */
    bool logged_in;                  /**< Login status */
    char *user;                      /**< Username */
    char *working_directory;         /**< Current working directory */
    data_socket_t *data_socket;      /**< Data socket configuration */
    transfer_mode_t transfer_mode;   /**< Current transfer mode */
} connection_t;

/**
 * @brief Command structure
 */
typedef struct command_s {
    char *name;  /**< Command name */
    int argc;    /**< Argument count */
    char **argv; /**< Argument array */
} command_t;

/**
 * @brief Command execution status enumeration
 */
typedef enum command_status_e {
    COMMAND_SUCCESS,   /**< Command executed successfully */
    COMMAND_FAILURE,   /**< Command failed */
    COMMAND_NOT_FOUND, /**< Command not recognized */
    COMMAND_QUIT,      /**< Client requested quit */
} command_status_t;

/**
 * @brief Command handler structure
 */
typedef struct command_handler_s {
    char *command_name; /**< Command name */
    command_status_t (*handler)(command_t *command,
        connection_t *connection); /**< Handler function */
} command_handler_t;

// Client Handler
/**
 * @brief Handles client connection and processes commands
 * @param fd Poll file descriptor
 * @param connection Client connection structure
 */
void handle_connection(struct pollfd *fd, connection_t *connection);

// Socket
/**
 * @brief Sets up server socket
 * @param port Port to listen on
 * @param path Server root directory path
 * @return Initialized server structure
 */
server_t setup_socket(int port, char *path);

// Connection
/**
 * @brief Process client connections
 * @param server Server configuration
 * @return Status code
 */
int process_connections(server_t *server);

// Command Parser
/**
 * @brief Parses input buffer into command structure
 * @param buffer Input buffer
 * @return Parsed command structure
 */
command_t *parse_buffer(char *buffer);

// Command Executor
/**
 * @brief Executes parsed command
 * @param command Command to execute
 * @param connection Client connection
 * @return Command execution status
 */
command_status_t execute_command(command_t *command, connection_t *connection);

// Utils
/**
 * @brief Converts string to uppercase
 * @param str String to convert
 * @return Modified string
 */
char *touppercase(char *str);

/**
 * @brief Merges two port numbers
 * @param ports Array of two port numbers
 * @return Merged port number
 */
int merge_port(int ports[2]);

/**
 * @brief Splits port number into components
 * @param port Port to split
 * @return Array of port components
 */
int *split_port(int port);

/**
 * @brief Replaces dots with commas in string
 * @param string String to modify
 * @return Modified string
 */
char *replace_dots_with_commas(char *string);

/**
 * @brief Removes leading/trailing whitespace
 * @param str String to trim
 * @return Trimmed string
 */
char *trim(char *str);

// Signal
/**
 * @brief Sets up signal handlers
 */
void setup_signal(void);

/**
 * @brief Checks if server should stop
 * @return Server stop status
 */
bool is_server_stopped(void);

// Destroyers
/**
 * @brief Cleans up connection resources
 * @param connection Connection to destroy
 * @param verbose Enable verbose output
 */
void destroy_connection(connection_t *connection, bool verbose);

/**
 * @brief Cleans up command resources
 * @param command Command to destroy
 */
void destroy_command(command_t *command);

/**
 * @brief Cleans up server resources
 * @param server Server to destroy
 * @param fds Poll file descriptors
 * @param connections Client connections
 */
void destroy_server(
    server_t *server, struct pollfd *fds, connection_t *connections);

/**
 * @brief Cleans up data socket resources
 * @param data_socket Data socket to destroy
 * @param verbose Enable verbose output
 */
void destroy_data_socket(data_socket_t *data_socket, bool verbose);

// Data Socket Setup
/**
 * @brief Sets up passive mode data socket
 * @return Configured data socket
 */
data_socket_t *setup_passive_data_socket(void);

/**
 * @brief Sets up active mode data socket
 * @param host_port Host port array
 * @return Configured data socket
 */
data_socket_t *setup_active_data_socket(int *host_port);

/**
 * @brief Executes data socket command
 * @param connection Client connection
 * @param command_execution Command execution function
 * @param command Command to execute
 * @return Command execution status
 */
command_status_t execute_data_socket_command(connection_t *connection,
    command_status_t (*command_execution)(connection_t *, command_t *),
    command_t *command);

// FTP Commands
/**
 * @brief USER command handler
 * @param command Command structure
 * @param connection Client connection
 * @return Command execution status
 */
command_status_t user_command(command_t *command, connection_t *connection);
command_status_t pass_command(command_t *command, connection_t *connection);
command_status_t cwd_command(command_t *command, connection_t *connection);
command_status_t cdup_command(command_t *command, connection_t *connection);
command_status_t quit_command(command_t *command, connection_t *connection);
command_status_t dele_command(command_t *command, connection_t *connection);
command_status_t pwd_command(command_t *command, connection_t *connection);
command_status_t pasv_command(command_t *command, connection_t *connection);
command_status_t port_command(command_t *command, connection_t *connection);
command_status_t help_command(command_t *command, connection_t *connection);
command_status_t noop_command(command_t *command, connection_t *connection);
command_status_t retr_command(command_t *command, connection_t *connection);
command_status_t stor_command(command_t *command, connection_t *connection);
command_status_t list_command(command_t *command, connection_t *connection);
command_status_t type_command(command_t *command, connection_t *connection);

#endif /* !MYFTP_H_ */
