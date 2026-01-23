#ifndef LOGGER_H
#define LOGGER_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define LOGGER_PROJECT_ID 'L'

typedef struct {
    long mtype;
    int sim_minutes;
    char sender[32];
    char text[128];
} log_msg_t;


void log_event(key_t key, const char* sender, const char* text, int sim_min);

void run_logger(key_t key);

#endif