
cmake_minimum_required(VERSION 2.8)
ENABLE_TESTING()


project(stave)

link_directories("/usr/local/lib")

# -stdlib=libc++
# -Werror
# -Weffc++ 
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11   -g -Wall -Wextra -Wno-unused-parameter -Wno-missing-braces -Weffc++")
# -stdlib=libc++
include_directories("include")
add_executable(stave src/main.cpp)
# target_link_libraries(ray "gd")


#find_package(Boost REQUIRED)

# include_directories(${BOOST_INCLUDE_DIR})
#include_directories("/opt/local/include" "/usr/local/include")

#target_link_libraries(ray ${BOOST_LIBRARY} boost_program_options boost_filesystem boost_system)

# nein! eigen
# include_directories("/opt/local/include/eigen3/")
# include_directories("/Users/iaefai/.work/eigen/")

#add_executable(test2 tests/linear_algebra.cxx)
#add_test(test ${CMAKE_CURRENT_BINARY_DIR}/test)

#add_executable(test3 tests/triangle_test.cxx src/geometry.cpp src/linear_algebra.cpp)

add_executable(product src/product.cpp)
