cmake_minimum_required(VERSION 3.11...3.26)
project(CurseDemos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

find_package(Curses REQUIRED)
add_library(Curses::Curses INTERFACE IMPORTED)
set_target_properties(
    Curses::Curses
    PROPERTIES
        INTERFACE_LINK_LIBRARIES "${CURSES_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${CURSES_INCLUDE_DIRS}"
)


function(make_exe_with_curses exe srcs)
    add_executable(${exe} ${srcs})
    target_link_libraries(${exe} PRIVATE Curses::Curses)
endfunction(make_exe_with_curses exe srcs)

add_subdirectory(sandbox)