/**
* @file driver.h
 * @brief Definitions for the Driver entity and Inter-Process Communication (IPC) messages.
 * * This file defines the car's lifecycle states, the main Driver structure
 * containing all repair data, and the message wrapper for System V IPC.
 */

#ifndef DRIVER_H
#define DRIVER_H

#define MAX_REPORTED_FAULTS 30

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "config.h"

/**
 * @enum CarState
 * @brief Represents the current stage of the vehicle within the workshop system.
 */
typedef enum {
    STATE_NEW,            /**< Vehicle just generated, waiting for Front Desk. */
    STATE_ESTIMATED,      /**< Front Desk has calculated costs and time. */
    STATE_IN_REPAIR,      /**< Vehicle is currently assigned to a Mechanic. */
    STATE_FINISHED,       /**< Service completed (all jobs processed or refused). */
    STATE_ABORTED         /**< Emergency interruption (e.g., workshop fire/evacuation). */
} CarState;

/**
 * @struct Driver
 * @brief The primary data structure representing a vehicle and its service record.
 * * This structure is passed between processes via message queues. It contains
 * data filled sequentially by the Generator, Front Desk, and Mechanic.
 */
typedef struct
{
    int pid_driver;                             /**< Process ID of the driver (useful for identification). */
    CarBrand brand;                             /**< Character representation of the car brand. */
    CarState current_state;                     /**< Current lifecycle state of the vehicle. */

    RepairJob jobs[MAX_REPORTED_FAULTS];        /**< List of all reported and discovered faults. */
    int total_jobs_count;                       /**< Total number of active jobs in the array. */

    /** @name Front Desk Estimations
     * Fields populated by the Front Desk process based on the master price list.
     * @{ */
    int estimated_cost;                         /**< Initial price estimation for the repair */
    int estimated_time;                         /**< Initial time estimation in minutes. */

    /** @name Mechanic Final Data
         * Fields populated or updated by the Mechanic during the actual repair.
         * @{ */
    int final_cost;                             /**< Final amount to be paid (includes discovered faults). */
    int actual_time_spent;                      /**< Total time spent on the lift in minutes. */

    bool is_repaired;                           /**< Final flag indicating if the car left the shop in working order. */

} Driver;

/**
 * @struct DriverMsg
 * @brief System V IPC message wrapper for the Driver structure.
 * * This structure must be used with msgsnd() and msgrcv() functions.
 */
typedef struct {
    long mtype;                                 /**< Message type */
    Driver driver_data;                        /**< The actual Driver data payload. */
} DriverMsg;

#endif //DRIVER_H
