//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_SUPERBLOCK_H
#include <stdio.h>
#define FILESYSTEM_SUPERBLOCK_H

struct Superblock {
    char filename[256];
    long int size;
    int inode_count;
    int block_count;
    int first_block;
    int block_size;
    int inode_size;
    int first_inode;
};
#endif //FILESYSTEM_SUPERBLOCK_H
