//
// Created by Jasson Rodríguez Méndez on 6/12/20.
//
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "DiskManagement.h"
#include "../FileManagement/Superblock.h"
#include "../FileManagement/Inode.h"
#include "../FileManagement/Block.h"

int bootDrive(char *fileName) {
    return -1;
};

int resetDrive(char *filename) {
    return -1;
};

int createDrive(char filename[256], int size, int block_size, int inode_count, char username[32]) {
    int first_block = sizeof(struct Superblock) + inode_count * sizeof(struct iNode);
    int block_count = (size - first_block) / block_size;

    struct Superblock superblock = {
            .size = size,
            .inode_count = inode_count,
            .block_count = block_count,
            .first_block = first_block,
            .block_size = block_size,
            .inode_size = sizeof(struct iNode),
            .first_inode = sizeof(struct Superblock)
    };

    memcpy(superblock.filename, filename, 256);

    time_t raw_time;
    time(&raw_time);

    struct iNode root = {
            .filename = "root",
            .type = DIRECTORY,
            .modified_datetime = raw_time,
            .created_datetime = raw_time,
            .size = 0
    };

    memcpy(root.owner, username, 32);

    struct iNode empty = {.type = EMPTY};

    struct Block *empty_block = malloc(block_size);
    empty_block->state = FREE;

    FILE * fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(&superblock, sizeof(struct Superblock), 1, fp);
        fwrite(&root, sizeof(struct iNode), 1, fp);

        for (int i = 0; i < inode_count - 1; i++) {
            fwrite(&empty, sizeof(struct iNode), 1, fp);
        }

        for(int i = 0; i < block_count; i++){
            fwrite(empty_block, block_size, 1, fp);
        }

        fclose(fp);
        return 0;
    }
    return 1;
};

int checkIntegrity(char filename[256]) {
    FILE * fp = fopen(filename, "rb");
    if (fp != NULL) {
        struct Superblock superblock;
        struct iNode root;

        fread(&superblock, sizeof(struct Superblock), 1, fp);
        fread(&root, sizeof(struct iNode), 1, fp);
        fclose(fp);

        printf("Volume name: %s\n", filename);
        printf("Size: %ld bytes\n", superblock.size);
        printf("iNode count: %d\n", superblock.inode_count);
        printf("Block count: %d\n", superblock.block_count);
        printf("First block: %d\n", superblock.first_block);
        printf("Block size: %d\n", superblock.block_size);
        printf("iNode size: %d\n", superblock.inode_size);
        printf("First iNode: %d\n", superblock.first_inode);
        printf("----------------------\n");
        printf("iNode 0: %s\n", root.filename);
        printf("Type: %d\n", root.type);
        printf("Owner: %s\n", root.owner);
        struct tm *modified = localtime(&(root.modified_datetime));
        printf("Modified: %s", asctime(modified));
        struct tm *created = localtime(&(root.created_datetime));
        printf("Created: %s", asctime(created));
        printf("Size: %d\n", root.size);
        printf("Continuation iNode: %d\n", root.continuation_iNode);
        printf("Parent iNode: %d\n", root.iNode_parent);
        return 0;
    }
    return 1;
};
