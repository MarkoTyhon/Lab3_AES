cmake_minimum_required(VERSION 3.15)
project(LAB3_AES VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

file("GLOB_RECURSE" "SOURCE_FILES" "src/*.cpp")
add_executable(main main.cpp ${SOURCE_FILES})
target_include_directories(main PUBLIC "./include")

if(UNIX)
	target_link_libraries(main ncurses)
endif(UNIX)