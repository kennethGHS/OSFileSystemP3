//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_BLOCK_H
#define FILESYSTEM_BLOCK_H
enum state_t{
    FREE,
    USED
};

struct Block{
    enum state_t state;
    char information[];
};
#endif //FILESYSTEM_BLOCK_H
