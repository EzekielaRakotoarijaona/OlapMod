cmake_minimum_required(VERSION 2.6)

project(algoNbEnregistrements)



if(USE_OpenMP)
  find_package(OpenMP REQUIRED)
endif(USE_OpenMP)

if (OPENMP_FOUND)
    include_directories("${OPENMP_INCLUDES}")
    link_directories("${OPENMP_LIBRARIES}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    # set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(OPENMP_FOUND)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main ../source/main/source/main.cpp ../source/main/source/nbEnregistrements.cpp ../source/main/source/calculBenefice.cpp)

add_executable(main_test ../source/test/source/main.cpp ../source/test/source/unit_test.cpp ../source/main/source/nbEnregistrements.cpp ../source/main/source/calculBenefice.cpp)

target_link_libraries(main PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(main_test PRIVATE OpenMP::OpenMP_CXX)


enable_testing()

add_test (test main_test)
set_tests_properties(test PROPERTIES PASS_REGULAR_EXPRESSION "Test réussi! \n")

set(CMAKE_INSTALL_PREFIX ..)