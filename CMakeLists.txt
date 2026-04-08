cmake_minimum_required( VERSION 4.1 )
project( lab_work_number2 )

set( CMAKE_CXX_STANDARD 20 )

# Подключаем gtests
include( FetchContent )
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable( googletest )

# Основная программа
add_executable( lab_work_number2
        main.cpp )

target_include_directories( lab_work_number2 PRIVATE src )

# Тесты
add_executable( tests
        tests/DynamicArray_tests.cpp
        tests/LinkedList_tests.cpp
        tests/Sequence_tests.cpp
        tests/BitSequence_tests.cpp
        tests/MapReduce_tests.cpp
)
target_include_directories( tests PRIVATE src )
target_link_libraries( tests gtest_main )

include( GoogleTest )
gtest_discover_tests( tests )
