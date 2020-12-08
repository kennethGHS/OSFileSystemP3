//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_FILERW_H
#define FILESYSTEM_FILERW_H

#include "FileDescriptor.h"
#include "Inode.h"

int set_working_drive(struct Drive *drive);

int set_current_user(char username[32]);

struct iNode *open_inode(char *filename, enum type_t type);

struct FileDescriptor *open_file(char *filename);

int create_dir(char *filename);

int write_file(struct FileDescriptor *fileDescriptor, char *data, int size);

int list_directories();
//int getFreeBlock();
//int getRootInfo();
//int setCursorToByte(int byteNumber);
//int searchDirectory(char ** listDirectories);
//int searchFromCurrent(char * filename);
//struct iNode * getInodeFilename();

#endif //FILESYSTEM_FILERW_H
