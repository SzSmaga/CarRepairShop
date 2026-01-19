

#ifndef CLOCK_H
#define CLOCK_H


#include <stdint.h>

/**
 * @file clock.h
 * @brief Simulation clock module.
 *
 * This module implements a virtual (simulated) clock that advances in discrete
 * ticks. The simulated time is independent from real wall-clock time, which
 * makes it suitable for pausable/resumable simulations (e.g., via checkpoints).
 */


/** @brief Number of simulated minutes added per single tick. */
#define SIM_MINUTES_PER_TICK 1


/** @brief Real-time duration of a single tick in milliseconds (used by the main loop). */
#define TICK_REAL_MS 100


/** @brief Number of minutes in a 24-hour day. */
#define MINUTES_PER_DAY 1440


/** @brief Service opening time (minutes from 00:00), e.g. 08:00 -> 480. */
#define T_p 480

/** @brief Service closing time (minutes from 00:00), e.g. 16:00 -> 960. */
#define T_k 960

/**
 * @struct sim_clock_t
 * @brief Simulation clock state.
 *
 * Holds the current simulated time (in minutes) and cached flags derived from it.
 * The simulated time can be saved to/loaded from a checkpoint file to allow the
 * simulation to resume without counting real-world downtime.
 */
typedef struct {

    /** Total simulated time in minutes since the simulation start. */
    uint64_t sim_minutes;

    /** Cached state: 1 if the service is currently open, 0 otherwise. */
    uint8_t is_open;

    /** Shutdown flag used by the main loop to terminate gracefully (0/1). */
    uint8_t shutdown;
} sim_clock_t;

/**
 * @brief Initializes the simulation clock.
 *
 * Sets the simulated time to @p start_minutes, clears the shutdown flag,
 * and updates cached derived fields (e.g., open/closed state).
 *
 * @param clk Pointer to the simulation clock structure to initialize.
 * @param start_minutes Initial simulated time in minutes since simulation start.
 *
 * @note If @p clk is NULL, the function returns immediately.
 */
void clock_init(sim_clock_t *clk, uint64_t start_minutes);


/**
 * @brief Advances the simulation clock by one tick.
 *
 * Increments simulated time by @c SIM_MINUTES_PER_TICK and updates cached flags
 * derived from the time (e.g., service open/closed state).
 *
 * @param clk Pointer to the simulation clock structure.
 *
 * @note If @p clk is NULL, the function returns immediately.
 */
void clock_tick(sim_clock_t *clk);


/**
 * @brief Returns whether the service is currently open.
 *
 * This function ensures the cached flag is consistent with the current simulated
 * time before returning it.
 *
 * @param clk Pointer to the simulation clock structure.
 * @return 1 if the service is open, 0 otherwise (also returns 0 if @p clk is NULL).
 */
int clock_is_open(sim_clock_t *clk);


/**
 * @brief Computes the number of simulated minutes until the next opening time.
 *
 * If the service is currently open, returns 0. Otherwise returns the number of
 * minutes until the next occurrence of @c T_p (later today or on the next day).
 *
 * @param clk Pointer to the simulation clock structure (read-only).
 * @return Minutes until the next opening time; 0 if currently open or @p clk is NULL.
 */
uint64_t clock_minutes_to_open(const sim_clock_t *clk);

/**
 * @brief Saves the current simulated time to a checkpoint file.
 *
 * The checkpoint is stored in a simple text format (single line with minutes),
 * allowing the simulation to resume from the same simulated time after restart.
 *
 * @param clk Pointer to the simulation clock structure (read-only).
 * @param path_name Path to the checkpoint file.
 * @return 0 on success, -1 on failure (with @c errno set accordingly).
 */
int clock_save(const sim_clock_t *clk, const char *path_name);


/**
 * @brief Loads the simulated time from a checkpoint file.
 *
 * Reads the checkpoint stored by @ref clock_save and updates cached fields
 * such as open/closed state and the shutdown flag.
 *
 * @param clk Pointer to the simulation clock structure to fill.
 * @param path_name Path to the checkpoint file.
 * @return 0 on success, -1 on failure (with @c errno set accordingly).
 */
int clock_load( sim_clock_t *clk, const char *path_name);


#endif //CLOCK_H
