#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void log_event(key_t key, const char* sender, const char* text, int sim_min) {

    int msgid = msgget(key, 0666);
    if (msgid == -1) return;

    log_msg_t msg;
    msg.mtype = 1;
    msg.sim_minutes = sim_min;
    strncpy(msg.sender, sender, 31);
    strncpy(msg.text, text, 127);

    msgsnd(msgid, &msg, sizeof(log_msg_t) - sizeof(long), 0);
}

void run_logger(key_t key) {
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Logger: msgget failed");
        exit(1);
    }

    FILE *f = fopen("serwis.log", "a");
    log_msg_t msg;

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(log_msg_t) - sizeof(long), 0, 0) == -1) {
            if (errno == EIDRM) break;
            continue;
        }
        printf("[%02d:%02d] [%s]: %s\n", msg.sim_minutes/60, msg.sim_minutes%60, msg.sender, msg.text);
        if (f) {
            fprintf(f, "[%02d:%02d] [%s]: %s\n", msg.sim_minutes/60, msg.sim_minutes%60, msg.sender, msg.text);
            fflush(f);
        }
    }
    if (f) fclose(f);
}