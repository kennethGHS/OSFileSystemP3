cmake_minimum_required(VERSION 3.5)

project(CLI LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
INCLUDE_DIRECTORIES(/usr/include)
LINK_DIRECTORIES(/usr/include)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB project_SRC
        "../FileSystem/*/*.h"
        "../FileSystem/*/*.c"
        )

if (ANDROID)
    add_library(CLI SHARED
            main.cpp
            mainwindow.cpp
            mainwindow.h
            mainwindow.ui
            )
else ()
    add_executable(CLI
            main.cpp
            qconsole.cpp
            qconsole.h
            mainwindow.cpp
            mainwindow.h
            mainwindow.ui
            ${project_SRC}
            )
endif ()

target_link_libraries(CLI PRIVATE Qt5::Widgets json-c)
