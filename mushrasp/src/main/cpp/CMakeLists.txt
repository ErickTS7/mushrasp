cmake_minimum_required(VERSION 3.10.2)
project(MushRASP)

add_library(
    mush-lib
    SHARED
    FridaDetect.cpp
    SecureAPI.cpp
    Log.cpp
    EmulatorDetect.cpp
    MushEnvironment.cpp
    root.cpp
    Signature.cpp
)

find_library(
    log-lib
    log
)

target_link_libraries(
    mush-lib
    ${log-lib}
)
