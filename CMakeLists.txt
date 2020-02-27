cmake_minimum_required(VERSION 3.15)
project(interest_rate_modelling)

set(CMAKE_CXX_STANDARD 20)


add_library(probability src/probability/state.h src/probability/state.cpp src/probability/time.h src/probability/time.cpp)


add_executable(test_probability src/test_probability/main.cpp)
target_link_libraries(test_probability probability)
target_include_directories(test_probability PRIVATE src)