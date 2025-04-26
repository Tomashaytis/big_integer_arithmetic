cmake_minimum_required(VERSION 3.10)
project(BigIntegerArithmetic)


set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG /INCREMENTAL")
endif()

add_subdirectory(lib)
add_subdirectory(app)