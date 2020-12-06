//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_FILERW_H
#define FILESYSTEM_FILERW_H
#include "Inode.h"
int writeBlocks(char * data, int amountData, int blocks);
int getFreeBlock();
int getRootInfo();
int setCursorToByte(int byteNumber);
int searchDirectory(char ** listDirectories);
int searchFromCurrent(char * filename);
struct iNode * getInodeFilename();
#endif //FILESYSTEM_FILERW_H
