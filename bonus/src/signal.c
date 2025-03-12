/*
** EPITECH PROJECT, 2025
** MyFTP
** File description:
** Signal File
*/

#include "myftp.h"

static bool stop_client = false;

bool is_client_stopped(void)
{
    return stop_client;
}

static void handle_signal(int sig)
{
    (void)sig;
    stop_client = true;
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
