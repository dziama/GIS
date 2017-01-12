cmake_minimum_required(VERSION 3.6)
project(GIS_master)

set(EXTRA_INCLUDE_PATHS)
set(EXTRA_LIBRARY_PATHS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
    GIS/Edge.cpp
    GIS/Edge.h
    GIS/FibonacciHeap.cpp
    GIS/FibonacciHeap.h
    GIS/Graph.cpp
    GIS/Graph.h
    GIS/HeapGui.cpp
    GIS/HeapGui.h
    GIS/HeapNode.cpp
    GIS/HeapNode.h
    GIS/HeapNodeGui.cpp
    GIS/HeapNodeGui.h
    GIS/Interfaces.h
    GIS/Links.h
    GIS/SFML.h
    GIS/Utility.cpp
    GIS/Utility.h
    GIS/Vertex.cpp
    GIS/Vertex.h
    )

set(APP_FILES
        ${SOURCE_FILES}
        GIS/GIS.cpp
        )

set(TEST_FILES
        ${SOURCE_FILES}
        GIS_tests/FibbonacciHeapTestClass.cpp
        GIS_tests/FibbonacciHeapTestClass.h
        GIS_tests/GIS_tests.cpp
        )

add_executable(app ${APP_FILES})
add_executable(tests ${TEST_FILES})

find_library(BOOST_FILESYSTEM_LIB boost_filesystem)
find_library(BOOST_TEST_LIB boost_unit_test_framework)
find_library(BOOST_SYSTEM_LIB boost_system)
find_library(BOOST_PROGRAM_OPTIONS_LIB boost_program_options)

find_library(SFML_GRAPHICS_LIB sfml-graphics)
find_library(SFML_SYSTEM_LIB sfml-system)
find_library(SFML_WINDOW_LIB sfml-window)

set(COMMON_LIBS
        ${BOOST_FILESYSTEM_LIB}
        ${BOOST_PROGRAM_OPTIONS_LIB}
        ${BOOST_SYSTEM_LIB}
        ${SFML_GRAPHICS_LIB}
        ${SFML_SYSTEM_LIB}
        ${SFML_WINDOW_LIB}
        )

include_directories(${CMAKE_SOURCE_DIR} ${EXTRA_INCLUDE_PATHS})
link_directories(${EXTRA_LIBRARY_PATHS})

target_link_libraries(app ${COMMON_LIBS})
target_link_libraries(tests ${COMMON_LIBS} ${BOOST_TEST_LIB})