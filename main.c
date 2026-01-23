#include "clock.h"
#include "logger.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

static volatile sig_atomic_t g_stop = 0;

static void handle_sigint(int sig)
{
    (void)sig;
    g_stop = 1;
}

static int setup_ctrl_c_handler(void)
{
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) != 0) {
        perror("setup_ctrl_c_handler: sigaction failed");
        return -1;
    }
    return 0;
}

static void sleep_tick(void)
{
    usleep(TICK_REAL_MS * 1000);
}

static const char* parse_state_file(int argc, char **argv)
{
    if (argc >= 2) return argv[1];
    return "clock.state";
}

int main(int argc, char **argv) {
    const char *state_file = parse_state_file(argc, argv);
    sim_clock_t clk;


    key_t log_key = ftok(argv[0], LOGGER_PROJECT_ID);
    if (log_key == -1) {
        perror("Main: ftok failed");
        return 1;
    }


    int msgid = msgget(log_key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Main: msgget failed");
        return 1;
    }


    pid_t logger_pid = fork();
    if (logger_pid == 0) {

        run_logger(log_key);
        exit(0);
    } else if (logger_pid < 0) {
        perror("Main: fork failed");
        msgctl(msgid, IPC_RMID, NULL);
        return 1;
    }




    if (setup_ctrl_c_handler() != 0) return 1;


    if (clock_load(&clk, state_file) != 0) {
        clock_init(&clk, T_p);
    }

    log_event(log_key, "SYSTEM", "Symulacja uruchomiona (Main)", (int)clk.sim_minutes);


    while (!g_stop) {
        clock_tick(&clk);


        if (clk.sim_minutes % 60 == 0) {
            log_event(log_key, "ZEGAR", "Minęła kolejna godzina", (int)clk.sim_minutes);
        }



        sleep_tick();
    }


    log_event(log_key, "SYSTEM", "Otrzymano sygnał stopu. Zamykanie...", (int)clk.sim_minutes);


    clock_save(&clk, state_file);


    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("Main: msgctl IPC_RMID failed");
    }

    waitpid(logger_pid, NULL, 0);

    printf("Symulacja zakończona poprawnie. Stan zapisany w %s\n", state_file);
    return 0;
}