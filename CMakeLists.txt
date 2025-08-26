cmake_minimum_required(VERSION 3.16)
project(slot_pirots3)
set(CMAKE_CXX_STANDARD 17)

# Indica on és el config de SFML instal·lat via MSYS2
# (comprova que aquesta ruta existeix al teu sistema)
set(SFML_DIR "C:/msys64/mingw64/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(slot_pirots3 main.cpp)
target_link_libraries(slot_pirots3 sfml-graphics sfml-window sfml-system)