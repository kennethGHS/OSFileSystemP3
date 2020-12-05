//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_INODESTRUCT_H
#define FILESYSTEM_INODESTRUCT_H
//
struct iNode{
    char filename[33];
    int blocks [15];
    char type;
    char state;
    char owner[33];
    int modifyH[5];
    int createdH[5];
    int size;
    int continuationInode;
    int iNodeOwner;
    int iNodeLocation;
};
struct iNode * actualInode;

#endif //FILESYSTEM_INODESTRUCT_H
