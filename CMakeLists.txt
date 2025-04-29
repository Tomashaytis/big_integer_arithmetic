cmake_minimum_required(VERSION 3.10)
project(BigIntegerArithmetic)

option(TESTING "Build the testing tree" ON)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG /INCREMENTAL")
    add_compile_options(/Zi /Od)
endif()

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(stat)

if(TESTING)
    message(STATUS "Enabling tests")
    enable_testing()
    add_subdirectory(test)
endif()