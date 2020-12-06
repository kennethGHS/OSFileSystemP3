//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_DISKMANAGEMENT_H
#define FILESYSTEM_DISKMANAGEMENT_H
int bootDrive(char * fileName);
int resetDrive(char  * filename);
int createDrive(int blocks, int blockSize,int filesize, int amountInodes);
int checkIntegrity(char * filename);
#endif //FILESYSTEM_DISKMANAGEMENT_H
