add_library(
    25lcxx_driver
    25LCxx_driver.c
    25LCxx_driver_basic.c
    25LCxx_driver_interface.c
    25LCxx_driver.h
    25LCxx_driver_basic.h
    25LCxx_driver_interface.h
)

target_include_directories(25lcxx_driver PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")