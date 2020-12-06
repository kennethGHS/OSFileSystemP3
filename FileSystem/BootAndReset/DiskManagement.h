//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_DISKMANAGEMENT_H
#define FILESYSTEM_DISKMANAGEMENT_H
int bootDrive(char * fileName);
int resetDrive(char  * filename);
int createDrive(char filename[256], int size, int block_size, int inode_count, char username[32]);
int checkIntegrity(char filename[256]);
#endif //FILESYSTEM_DISKMANAGEMENT_H
