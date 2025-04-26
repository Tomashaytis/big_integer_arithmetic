cmake_minimum_required(VERSION 3.14)

#find_package(Catch2 REQUIRED)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_executable(bint_tests src/test_bintlib.cpp)

target_include_directories(bint_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/include
)

target_link_libraries(bint_tests PRIVATE 
    bintlib
    Catch2::Catch2WithMain
)

enable_testing()


include(CTest)
include(Catch)
catch_discover_tests(bint_tests)