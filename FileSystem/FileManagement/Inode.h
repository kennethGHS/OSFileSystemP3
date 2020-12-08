//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_INODESTRUCT_H
#define FILESYSTEM_INODESTRUCT_H

#include <time.h>

enum type_t {
    EMPTY,
    DIRECTORY_START,
    DIRECTORY_EXTENSION,
    FILE_START,
    FILE_EXTENSION
};

struct iNode {
    char filename[256];
    enum type_t type;
    char owner[32];
    time_t modified_datetime;
    time_t created_datetime;
    unsigned long size;
    unsigned long continuation_iNode;
    unsigned long iNode_parent;
    unsigned long blocks[15];
};

#endif //FILESYSTEM_INODESTRUCT_H
