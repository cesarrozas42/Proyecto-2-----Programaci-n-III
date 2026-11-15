cmake_minimum_required(VERSION 3.16)
project(ArbolStrings CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 1. Buscar las librerías de hilos del sistema (Vital para tu parte)
find_package(Threads REQUIRED)

# 2. Agregamos TrieConcurrente.cpp a la librería base
add_library(arbol src/Trie.cpp src/TrieConcurrente.cpp)
target_include_directories(arbol PUBLIC src)

# 3. Enlazamos la librería de hilos a nuestro árbol
target_link_libraries(arbol PUBLIC Threads::Threads)

add_executable(pruebas src/pruebas.cpp)
target_link_libraries(pruebas PRIVATE arbol)

add_executable(demo src/demo.cpp)
target_link_libraries(demo PRIVATE arbol)

# 4. NUEVO: El ejecutable del Benchmark (Tu parte)
add_executable(benchmark src/benchmark.cpp)
target_link_libraries(benchmark PRIVATE arbol)