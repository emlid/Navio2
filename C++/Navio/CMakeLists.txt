cmake_minimum_required(VERSION 2.8.3)
project(navio)
add_definitions(-std=c++11)

include_directories(
	Common
	Navio+
	Navio2
)


set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(GNUInstallDirs)

set(SOURCE_FILES
	Common/I2Cdev.cpp
	Common/MPU9250.cpp
	Common/MS5611.cpp
	Common/Ublox.cpp
	Common/Util.cpp
	Common/gpio.cpp
	Navio2/ADC_Navio2.cpp
	Navio2/LSM9DS1.cpp
	Navio2/Led_Navio2.cpp
	Navio2/PWM.cpp
	Navio2/RCInput_Navio2.cpp
	Navio2/RCOutput_Navio2.cpp
	Navio2/RGBled.cpp
	)

add_library(navio SHARED ${SOURCE_FILES})
target_include_directories(navio PUBLIC .)
# set_target_properties(navio PROPERTIES PUBLIC_HEADER ${HEADER_FILES})
install(TARGETS navio
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/" # source directory
        DESTINATION "include" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
)
