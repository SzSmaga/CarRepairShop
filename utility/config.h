/**
* @file config.h
 * @brief Configuration constants, enums, and structures for the workshop simulation.
 * * This file contains the master definitions for fault types, job statuses,
 * and repair job structures used across the Driver, Front Desk, and Mechanic processes.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

/**
 * @brief Type alias for car brands, represented by characters (e.g., 'A', 'E', 'U').
 */
typedef char CarBrand;

/**
 * @enum FaultID
 * @brief Unique identifiers for all possible car faults.
 * * @note Faults 0-2 (CRITICAL) can be processed outside regular working hours.
 * @note Faults 3-29 are standard maintenance/repair tasks.
 */
typedef enum {
    /* CRITICAL FAULTS (0-2) */
    FAULT_BRAKE_SYSTEM_FAILURE = 0,
    FAULT_ENGINE_FIRE_DAMAGE = 1,
    FAULT_STEERING_COLUMN_LOCK = 2,

    /* STANDARD FAULTS (3-29) */
    FAULT_OIL_CHANGE = 3,
    FAULT_BRAKE_PADS_REPLACEMENT = 4,
    FAULT_SUSPENSION_REPAIR = 5,
    FAULT_TIMING_BELT_REPLACEMENT = 6,
    FAULT_AC_RECHARGE = 7,
    FAULT_TIRE_REPLACEMENT = 8,
    FAULT_WHEEL_ALIGNMENT = 9,
    FAULT_BATTERY_REPLACEMENT = 10,
    FAULT_ALTERNATOR_REPAIR = 11,
    FAULT_SPARK_PLUGS_REPLACEMENT = 12,
    FAULT_EXHAUST_SYSTEM_REPAIR = 13,
    FAULT_CLUTCH_REPLACEMENT = 14,
    FAULT_TURBOCHARGER_REGENERATION = 15,
    FAULT_FILTER_SET_REPLACEMENT = 16,
    FAULT_STARTER_MOTOR_REPAIR = 17,
    FAULT_BULB_REPLACEMENT = 18,
    FAULT_FUEL_INJECTOR_CLEANING = 19,
    FAULT_ECU_SOFTWARE_REPAIR = 20,
    FAULT_RADIATOR_REPLACEMENT = 21,
    FAULT_GEARBOX_OVERHAUL = 22,
    FAULT_WATER_PUMP_REPLACEMENT = 23,
    FAULT_DPF_CLEANING = 24,
    FAULT_SHOCK_ABSORBER_REPLACEMENT = 25,
    FAULT_CENTRAL_LOCK_REPAIR = 26,
    FAULT_WINDSHIELD_REPLACEMENT = 27,
    FAULT_ABS_MODULE_REPAIR = 28,
    FAULT_WHEEL_BEARING_REPLACEMENT = 29
} FaultID;

/**
 * @enum JobStatus
 * @brief Represents the lifecycle state of a specific repair task.
 */
typedef enum {
    JOB_PENDING,    /**< Task is waiting for decision or assigned to a mechanic. */
    JOB_DONE,       /**< Task completed successfully. */
    JOB_REFUSED,    /**< Customer denied consent for this specific repair. */
    JOB_FAILED      /**< Repair interrupted (e.g., due to workshop emergency/fire). */
} JobStatus;

/**
 * @struct FaultDefinition
 * @brief Static data for a fault type as defined in the master price list.
 */
typedef struct {
    int id;             /**< Unique FaultID. */
    char name[40];      /**< Human-readable description of the fault. */
    int base_price;     /**< Standard cost. */
    int base_time;      /**< Estimated duration in minutes. */
    bool is_critical;   /**< Flag indicating if the fault allows after-hours entry. */
} FaultDefinition;

/**
 * @struct RepairJob
 * @brief Instance of a repair task assigned to a specific vehicle.
 * * This structure tracks whether the fault was known by the driver or discovered
 * during inspection, and whether the repair was actually carried out.
 */
typedef struct {
    FaultID fault_id;
    bool reported_by_driver; /**< Reference to the fault definition. */
    bool fix_confirmed;      /**< True if the driver disclosed the fault at Front Desk. */
    bool adds_time;          /**< True if the customer agreed to the repair/cost. */
    JobStatus status;        /**< Current progress of this specific job. */
} RepairJob;

/**
 * @var price_list
 * @brief The global catalog of all supported faults and their parameters.
 */
extern FaultDefinition price_list[30];

#endif //CONFIG_H
