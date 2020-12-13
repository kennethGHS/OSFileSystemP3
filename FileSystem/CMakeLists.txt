cmake_minimum_required(VERSION 3.17)
project(FileSystem C)

set(CMAKE_C_STANDARD 99)
INCLUDE_DIRECTORIES(/usr/include)
LINK_DIRECTORIES(/usr/include)
#SET(CMAKE_CXX_FLAGS   -ljson )
add_executable(FileSystem main.c FileManagement/FileRW.c FileManagement/Inode.h FileManagement/FileValidantions.h FileManagement/Block.h FileManagement/Superblock.h BootAndReset/DiskManagement.c Json/json_creation.h Json/json_creation.c main.c)
TARGET_LINK_LIBRARIES(FileSystem json-c)