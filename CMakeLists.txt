cmake_minimum_required(VERSION 3.16)

project(NetworkLatencyPacketLossChaosEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chaos_emulator
    src/main.cpp
)

add_executable(driver_ioctl_test
    tests/driver_ioctl_test.cpp
)

add_executable(tcp_test
    tests/tcp_test.cpp
)

add_executable(udp_test
    tests/udp_test.cpp
)
