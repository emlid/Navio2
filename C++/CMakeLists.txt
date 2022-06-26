cmake_minimum_required(VERSION 2.8.3)
project(navio2_cpp)

set (CMAKE_CXX_STANDARD 14)

add_subdirectory(Navio)

include_directories(Examples)

set(EXMPL_SRCS
        AccelGyroMag.cpp
        ADC.cpp
        AHRS.cpp
        Barometer.cpp
        LED.cpp
        RCInput.cpp
        Servo.cpp
        threaded_baro.cpp)

foreach( testsourcefile ${EXMPL_SRCS} )
    string( REPLACE ".cpp" "" testname ${testsourcefile} )
    add_executable( ${testname} Examples/${testsourcefile} )
    target_link_libraries( ${testname} navio pigpio pthread )
endforeach( testsourcefile ${EXMPL_SRCS} )


