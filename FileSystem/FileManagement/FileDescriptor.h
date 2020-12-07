//
// Created by Jasson Rodríguez Méndez on 6/12/20.
//

#ifndef FILESYSTEM_FILEDESCRIPTOR_H
#define FILESYSTEM_FILEDESCRIPTOR_H

struct FileDescriptor {
    struct Drive *drive;
    struct iNode *inode;
    int cursor;
};

#endif //FILESYSTEM_FILEDESCRIPTOR_H
