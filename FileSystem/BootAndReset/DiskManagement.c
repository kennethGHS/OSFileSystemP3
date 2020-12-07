//
// Created by Jasson Rodríguez Méndez on 6/12/20.
//
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "DiskManagement.h"
#include "../BootAndReset//Drive.h"
#include "../FileManagement/Superblock.h"
#include "../FileManagement/Inode.h"
#include "../FileManagement/Block.h"

int bootDrive(char *fileName) {
    return -1;
};

int resetDrive(char *filename) {
    return -1;
};

struct Drive *createDrive(char filename[256], int size, int block_size, int inode_count, char username[32]) {
    int first_block = sizeof(struct Superblock) + inode_count * sizeof(struct iNode);
    int block_count = (size - first_block) / block_size;

    time_t raw_time;
    time(&raw_time);

    struct Drive *drive = malloc(sizeof(struct Drive));

    drive->superblock.size = size;
    drive->superblock.inode_count = inode_count;
    drive->superblock.block_count = block_count;
    drive->superblock.first_block = first_block;
    drive->superblock.block_size = block_size;
    drive->superblock.inode_size = sizeof(struct iNode);
    drive->superblock.first_inode = sizeof(struct Superblock);

    drive->root.type = DIRECTORY_START;
    drive->root.modified_datetime = raw_time;
    drive->root.created_datetime = raw_time;
    drive->root.size = 0;
    drive->root.continuation_iNode = 0;

    memcpy(drive->superblock.filename, filename, 256);
    memcpy(drive->root.filename, "root", 256);
    memcpy(drive->root.owner, username, 32);
    memset(drive->root.blocks, 0, 15);

    struct iNode empty = {.type = EMPTY};

    struct Block *empty_block = malloc(block_size);
    empty_block->state = FREE;

    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(drive, sizeof(struct Drive), 1, fp);

        for (int i = 0; i < inode_count - 1; i++) {
            fwrite(&empty, sizeof(struct iNode), 1, fp);
        }

        for (int i = 0; i < block_count; i++) {
            fwrite(empty_block, block_size, 1, fp);
        }

        fclose(fp);
        return drive;
    }
    return NULL;
};

int checkIntegrity(char filename[256]) {
    FILE *fp = fopen(filename, "rb");
    if (fp != NULL) {
        struct Drive drive;

        fread(&drive, sizeof(struct Drive), 1, fp);
        fclose(fp);

        printf("Volume name: %s\n", filename);
        printf("Size: %ld bytes\n", drive.superblock.size);
        printf("iNode count: %d\n", drive.superblock.inode_count);
        printf("Block count: %d\n", drive.superblock.block_count);
        printf("First block: %d\n", drive.superblock.first_block);
        printf("Block size: %d\n", drive.superblock.block_size);
        printf("iNode size: %d\n", drive.superblock.inode_size);
        printf("First iNode: %d\n", drive.superblock.first_inode);
        printf("----------------------\n");
        printf("iNode 0: %s\n", drive.root.filename);
        printf("Type: %d\n", drive.root.type);
        printf("Owner: %s\n", drive.root.owner);
        struct tm *modified = localtime(&(drive.root.modified_datetime));
        printf("Modified: %s", asctime(modified));
        struct tm *created = localtime(&(drive.root.created_datetime));
        printf("Created: %s", asctime(created));
        printf("Size: %d\n", drive.root.size);
        printf("Continuation iNode: %d\n", drive.root.continuation_iNode);
        printf("Parent iNode: %d\n", drive.root.iNode_parent);
        return 0;
    }
    return 1;
};
