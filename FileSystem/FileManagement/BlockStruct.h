//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_BLOCKSTRUCT_H
#define FILESYSTEM_BLOCKSTRUCT_H
struct block{
    int blockNumber;
    int state;
    struct block * nextBlock;
};
#endif //FILESYSTEM_BLOCKSTRUCT_H
