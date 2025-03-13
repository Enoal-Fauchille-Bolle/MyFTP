/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Signal File
*/

#include "myftp.h"

/** @brief Global flag to indicate if the server should stop */
static volatile sig_atomic_t stop_server = false;

/**
 * @brief Checks if the server has received a stop signal
 *
 * @return bool true if the server should stop, false otherwise
 */
bool is_server_stopped(void)
{
    return stop_server;
}

/**
 * @brief Signal handler for SIGINT and SIGTERM
 *
 * Sets the stop_server flag to true and prints a message indicating
 * that the server is stopping.
 *
 * @param sig Signal number (unused)
 */
static void handle_signal(int sig)
{
    (void)sig;
    printf("\nStopping FTP server...\n");
    stop_server = true;
}

/**
 * @brief Sets up signal handlers for graceful server shutdown
 *
 * Configures handlers for SIGINT and SIGTERM signals to allow
 * graceful shutdown of the server when these signals are received.
 */
void setup_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}
