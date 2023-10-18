cmake_minimum_required(VERSION 3.26)
project(WordGame)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE ON)  # output makefile

set(Wt_DIR /wt-4.10.1)
set(Boost_DIR /boost_1_83_0)
find_package(Wt)
find_package(Boost)

add_compile_options()
add_executable(WordGame WordGame.cpp
        main.cpp
        WordGame.h
        WordGame.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        Wt::Wt
        Wt::HTTP
        Boost::boost
)

