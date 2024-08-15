cmake_minimum_required(VERSION 3.30)

project(sees VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(ENABLE_QML_DEBUG "enable qml debug" OFF)
if (${ENABLE_QML_DEBUG})
	message(STATUS "flags: ${CMAKE_CXX_FLAGS_DEBUG}")
	string(APPEND CMAKE_CXX_FLAGS_DEBUG " -DQT_QML_DEBUG")
	message(STATUS "flags: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

find_package(Qt6 6.7 REQUIRED COMPONENTS Core Quick Qml Network Sql)
qt_standard_project_setup()

add_subdirectory(third_party/mpvqt)
add_subdirectory(app)
