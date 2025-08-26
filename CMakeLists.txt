cmake_minimum_required(VERSION 3.16)
project(slot_pirots3)
set(CMAKE_CXX_STANDARD 17)

# --- SFML via MSYS2 ---
set(SFML_DIR "C:/msys64/mingw64/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Includes
include_directories(include)

# --- Main game (SFML) ---
add_executable(slot_pirots3
        src/main.cpp
        src/game/grid.cpp
        src/game/layout.cpp
        src/game/math.cpp
        include/game/layout.hpp
)
target_link_libraries(slot_pirots3 sfml-graphics sfml-window sfml-system)

# --- RTP headless tool (no graphics) ---
add_executable(rtp_test
        tests/test_math.cpp
        src/game/grid.cpp
        src/game/math.cpp
        tests/test_math.cpp
)
target_link_libraries(rtp_test sfml-system)
