cmake_minimum_required(VERSION 3.17)
project(FileSystem C)

set(CMAKE_C_STANDARD 99)

add_executable(FileSystem main.c FileManagement/FileRW.h FileManagement/Inode.h FileManagement/FileValidantions.h FileManagement/Block.h FileManagement/Superblock.h BootAndReset/DiskManagement.c)