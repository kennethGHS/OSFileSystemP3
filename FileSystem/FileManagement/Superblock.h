//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_SUPERBLOCK_H
#define FILESYSTEM_SUPERBLOCK_H
#include <stdio.h>

struct Superblock {
    char filename[256];
    unsigned long size;
    unsigned int inode_count;
    unsigned int block_count;
    unsigned long first_block;
    unsigned int block_size;
    unsigned int inode_size;
    unsigned long first_inode;
};
#endif //FILESYSTEM_SUPERBLOCK_H
