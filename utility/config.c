/**
* @file config.c
 * @brief Implementation of global configuration data for the workshop.
 * * This file contains the actual data for the master price list used by
 * the Front Desk for estimations and by the Mechanic for repairs.
 */

#include "config.h"



/**
 * @var price_list
 * @brief Master database containing all 30 predefined fault types.
 * * @details Each entry includes a unique ID, human-readable name,
 * base repair cost in PLN, base time requirement in minutes,
 * and a flag indicating if the fault is considered critical.
 */
FaultDefinition price_list[30] = {
    /* ID, Name, Price (PLN), Time (min), IsCritical */
    {0, "Brake System Critical Failure", 1500, 240, true},
    {1, "Engine Compartment Fire Damage", 6000, 600, true},
    {2, "Total Steering Column Lock", 1800, 300, true},

    {3, "Standard Oil & Filter Change", 250, 45, false},
    {4, "Brake Pads Replacement", 400, 60, false},
    {5, "Front Suspension Repair", 900, 150, false},
    {6, "Timing Belt Kit Replacement", 1400, 360, false},
    {7, "Air Conditioning Service", 350, 90, false},
    {8, "Full Tire Set Replacement", 200, 60, false},
    {9, "Four-Wheel Alignment", 250, 45, false},
    {10, "Battery Replacement & Coding", 150, 20, false},
    {11, "Alternator Refurbishment", 500, 120, false},
    {12, "Spark Plugs Set Replacement", 180, 40, false},
    {13, "Exhaust Muffler Repair", 350, 80, false},
    {14, "Transmission Clutch Replacement", 2000, 420, false},
    {15, "Turbocharger Inspection & Repair", 2800, 300, false},
    {16, "Air and Cabin Filter Service", 120, 30, false},
    {17, "Starter Motor Replacement", 450, 100, false},
    {18, "Exterior Lighting Bulb Swap", 40, 15, false},
    {19, "Fuel Injectors Ultrasonic Cleaning", 700, 180, false},
    {20, "Engine Control Unit (ECU) Diagnostics", 1100, 120, false},
    {21, "Engine Radiator Replacement", 600, 160, false},
    {22, "Manual Gearbox Overhaul", 4000, 720, false},
    {23, "Engine Water Pump Replacement", 500, 140, false},
    {24, "Diesel Particulate Filter (DPF) Service", 950, 240, false},
    {25, "Rear Shock Absorbers Replacement", 750, 120, false},
    {26, "Central Locking System Repair", 300, 90, false},
    {27, "Front Windshield Replacement", 1000, 180, false},
    {28, "ABS Module Electronic Repair", 1200, 200, false},
    {29, "Front Wheel Bearing Replacement", 350, 110, false}
};