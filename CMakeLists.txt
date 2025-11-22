cmake_minimum_required (VERSION 3.10)

project ("Project")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG -g3)

set(SOURCES
  src/main.c
  src/util.c
  src/customer.c
  src/tarif.c
)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE include)