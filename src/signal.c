/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Signal File
*/

#include "myftp.h"

static volatile sig_atomic_t stop_server = false;

bool is_server_stopped(void)
{
    return stop_server;
}

static void handle_signal(int sig)
{
    (void)sig;
    printf("\nStopping FTP server...\n");
    stop_server = true;
}

void setup_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}
