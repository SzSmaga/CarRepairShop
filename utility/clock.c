

#include "clock.h"
#include <stdio.h>
#include <errno.h>


static void clock_update_open_flag(sim_clock_t *clk){

    uint64_t minute_of_day = clk->sim_minutes % MINUTES_PER_DAY;

    if (minute_of_day >= T_p && minute_of_day < T_k){
         clk->is_open = 1;
    }
    else{
        clk->is_open = 0;
    }
}

void clock_init(sim_clock_t *clk, uint64_t start_minutes)
{
    if (!clk) {
        return;
    }

    clk->sim_minutes = start_minutes;
    clk->shutdown = 0;
    clock_update_open_flag(clk);
}

void clock_tick(sim_clock_t *clk)
{
    if (!clk) {
        return;
    }

    clk->sim_minutes += SIM_MINUTES_PER_TICK;
    clock_update_open_flag(clk);
}

int clock_is_open(sim_clock_t *clk)
{
    if (!clk) {
        return 0;
    }

    /* Gwarantujemy spójność flagi nawet jeśli ktoś zmieni sim_minutes ręcznie */
    clock_update_open_flag(clk);
    return (int)clk->is_open;
}

uint64_t clock_minutes_to_open(const sim_clock_t *clk)
{
    if (!clk) {
        return 0;
    }

    uint64_t minute_of_day = clk->sim_minutes % MINUTES_PER_DAY;

    /* Jeśli otwarte, to do otwarcia jest 0 */
    if (minute_of_day >= T_p && minute_of_day < T_k) {
        return 0;
    }

    /* Przed otwarciem tego samego dnia */
    if (minute_of_day < T_p) {
        return (uint64_t)(T_p - minute_of_day);
    }

    /* Po zamknięciu: do końca doby + do Tp */
    return (uint64_t)((MINUTES_PER_DAY - minute_of_day) + T_p);
}

int clock_save(const sim_clock_t *clk, const char *path_name)
{
    if (!clk || !path_name) {
        errno = EINVAL;
        perror("clock_save: invalid argument");
        return -1;
    }

    FILE *f = fopen(path_name, "w");
    if (!f) {
        perror("clock_save: fopen failed");
        return -1;
    }


    if (fprintf(f, "%llu\n", (unsigned long long)clk->sim_minutes) < 0) {
        perror("clock_save: fprintf failed");
        fclose(f);
        return -1;
    }

    if (fclose(f) != 0) {
        perror("clock_save: fclose failed");
        return -1;
    }

    return 0;
}
int clock_load(sim_clock_t *clk, const char *path_name)
{
    if (!clk || !path_name) {
        errno = EINVAL;
        perror("clock_load: invalid argument");
        return -1;
    }

    FILE *f = fopen(path_name, "r");
    if (!f) {
        perror("clock_load: fopen failed");
        return -1;
    }

    unsigned long long minutes = 0;
    if (fscanf(f, "%llu", &minutes) != 1) {
        perror("clock_load: fscanf failed");
        fclose(f);
        return -1;
    }

    if (fclose(f) != 0) {
        perror("clock_load: fclose failed");
        return -1;
    }

    clk->sim_minutes = (uint64_t)minutes;
    clk->shutdown = 0;
    clock_update_open_flag(clk);

    return 0;
}