cmake_minimum_required(VERSION 3.15)
project(interest_rate_modelling)

set(CMAKE_CXX_STANDARD 20)

##### coverage
# add_compile_options(--coverage)
# set (COVERAGE_LINK_FLAG gcov)

add_library(probability src/probability/state.h src/probability/time.h src/probability/wiener_process.h src/probability/path.h src/probability/fwd_decl.h src/probability/path.cpp src/probability/state.cpp src/probability/time.cpp src/probability/wiener_process.cpp src/probability/wiener_process_template_defn.h)


add_executable(test_probability src/test_probability/main.cpp)
target_link_libraries(test_probability probability ${COVERAGE_LINK_FLAG})
target_include_directories(test_probability PRIVATE src)
