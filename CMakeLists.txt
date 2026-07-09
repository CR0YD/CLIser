cmake_minimum_required(VERSION 3.31)
project(cliser_lib C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -pedantic-errors -Wall -Wextra -Werror")

include_directories(include)

add_subdirectory(src)
add_subdirectory(tests)
