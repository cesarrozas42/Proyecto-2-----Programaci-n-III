cmake_minimum_required(VERSION 3.16)
project(ArbolStrings CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Biblioteca con el árbol: los demás integrantes enlazan contra "arbol".
add_library(arbol src/Trie.cpp)
target_include_directories(arbol PUBLIC src)

add_executable(pruebas src/pruebas.cpp)
target_link_libraries(pruebas PRIVATE arbol)

add_executable(demo src/demo.cpp)
target_link_libraries(demo PRIVATE arbol)
