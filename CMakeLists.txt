cmake_minimum_required(VERSION 3.5)

project(CLI LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB project_SRC
        "FileSystem/*/*.h"
        "FileSystem/*/*.c"
        "CLI/*.h"
        "CLI/*.cpp"
        )

add_executable(CLI
        CLI/mainwindow.ui
        ${project_SRC}
        )

target_link_libraries(CLI PRIVATE Qt5::Widgets json-c)
