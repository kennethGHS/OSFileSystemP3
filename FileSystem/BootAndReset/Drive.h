//
// Created by Jasson Rodríguez Méndez on 6/12/20.
//

#ifndef FILESYSTEM_DRIVE_H
#define FILESYSTEM_DRIVE_H

#include "../FileManagement/Superblock.h"
#include "../FileManagement/Inode.h"

struct Drive{
    struct Superblock superblock;
    struct iNode root;
};

#endif //FILESYSTEM_DRIVE_H
