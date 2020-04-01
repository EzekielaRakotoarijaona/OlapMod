cmake_minimum_required(VERSION 2.6)

project(algoNbEnregistrements)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main ../source/main/source/main.cpp ../source/main/source/nbEnregistrements.cpp)

add_executable(main_test ../source/test/source/main.cpp ../source/test/source/unit_test.cpp)

enable_testing()

add_test (test main_test)
set_tests_properties(test PROPERTIES PASS_REGULAR_EXPRESSION "Test réussi!\n")

set(CMAKE_INSTALL_PREFIX ..)