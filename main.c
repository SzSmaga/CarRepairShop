#include "clock.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


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

int main(int argc, char **argv)
{
    const char *state_file = parse_state_file(argc, argv);
    sim_clock_t clk;

    if (setup_ctrl_c_handler() != 0) {
        return 1;
    }

    /* Wznowienie lub start */
    if (clock_load(&clk, state_file) != 0) {
        if (errno == ENOENT) {
            fprintf(stderr, "main: no state file '%s', starting from 0\n", state_file);
        } else {
            fprintf(stderr, "main: failed to load '%s', starting from 0\n", state_file);
        }
        clock_init(&clk, 0);
    }

    unsigned long long tick_count = 0;

    while (!g_stop && !clk.shutdown) {
        sleep_tick();
        clock_tick(&clk);

        tick_count++;

        if (tick_count % 50 == 0) {
            clock_save(&clk, state_file);
        }
    }

    clock_save(&clk, state_file);
    fprintf(stderr, "main: exiting at sim_minutes=%llu\n",
            (unsigned long long)clk.sim_minutes);

    return 0;
}
