project(tests)

# '.lib' is the folder with Google Test sources
add_subdirectory(.lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tests BFMachineTest.cpp)
target_link_libraries(tests gtest gtest_main)
