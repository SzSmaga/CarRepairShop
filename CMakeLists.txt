cmake_minimum_required(VERSION 3.18)
project(CarRepairShop)

set(CMAKE_C_STANDARD 11)


add_executable(CarRepairShop
        main.c
        utility/clock.c
        utility/logger.c
        utility/logger.h
        utility/config.h
        utility/driver.h
        utility/frontdesk.h
        utility/menager.h
        utility/config.c
)

target_include_directories(CarRepairShop PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/utility
)