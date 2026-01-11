cmake_minimum_required(VERSION 3.15)
project(ML-library VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Source files
set(SOURCES
    src/trace.cpp
)

# Header files
set(HEADERS
    src/device.h
    src/tensor.h
    src/trace.h
)

# Create library
add_library(ml_library ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(ml_library PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link required libraries
target_link_libraries(ml_library PRIVATE pthread)
