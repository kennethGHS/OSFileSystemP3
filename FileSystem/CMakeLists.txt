cmake_minimum_required(VERSION 3.17)
project(FileSystem C)

set(CMAKE_C_STANDARD 99)

add_executable(FileSystem main.c FileManagement/FileRW.h FileManagement/InodeStruct.h FileManagement/FileValidantions.h FileManagement/BlockStruct.h FileManagement/DiskData.h BootAndReset/DiskManagement.h)