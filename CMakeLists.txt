cmake_minimum_required(VERSION 3.10)
project("RPG CPP Remake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

set(PROJECT_TARGET rpg_cpp_remake)
add_executable(${PROJECT_TARGET} src/main.cpp)

if(APPLE)
    target_link_libraries(${PROJECT_TARGET} raylib "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
else()
    target_link_libraries(${PROJECT_TARGET} raylib)
endif()
