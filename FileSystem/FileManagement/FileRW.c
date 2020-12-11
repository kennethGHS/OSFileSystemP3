//
// Created by Jasson Rodríguez Méndez on 6/12/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Inode.h"
#include "../BootAndReset/Drive.h"
#include "FileDescriptor.h"
#include "Block.h"

struct Drive *working_drive;
unsigned long cwd_index;
FILE *drive_image;
char user[32];

int set_working_drive(struct Drive *drive) {
    working_drive = drive;
    cwd_index = working_drive->superblock.first_inode;
    drive_image = fopen(drive->superblock.filename, "rb+");
    if (drive_image == NULL) {
        printf("Error: Can't open drive.");
        return 1;
    }
    return 0;
};

int set_current_user(char username[32]) {
    memcpy(user, username, 32);
    return 0;
}

// TODO check that all intermediate inodes are directories
struct iNode *open_inode(char *filename, enum type_t type) {
    // Select current directory to search from
    struct iNode *current_dir = malloc(sizeof(struct iNode));
    unsigned long cur_inode_index;
    if (filename[0] == '/') {
        cur_inode_index = working_drive->superblock.first_inode;
    } else {
        cur_inode_index = cwd_index;
    }
    fseek(drive_image, cur_inode_index, SEEK_SET);
    fread(current_dir, sizeof(struct iNode), 1, drive_image);

    // Split path
    char *cur_filename = strtok(filename, "/");
    char *next_filename = strtok(NULL, "/");
    struct iNode *next_dir = malloc(sizeof(struct iNode));
    int found;
    int had_to_be_created = 0;

    while (cur_filename != NULL) {
        found = 0;
        // Search current filename in current_dir to get next dir
        unsigned long block_index;
        for (int i = 0; i < 15; i++) {
            block_index = current_dir->blocks[i];
            if (block_index != 0) {
                fseek(drive_image, block_index, SEEK_SET);
                fread(next_dir, sizeof(struct iNode), 1, drive_image);
                if (strcmp(next_dir->filename, cur_filename) == 0) {
                    current_dir = next_dir;
                    cur_inode_index = block_index;
                    cur_filename = next_filename;
                    next_filename = strtok(NULL, "/");
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            if (current_dir->continuation_iNode != 0) {
                // Check continuation_inode
                cur_inode_index = current_dir->continuation_iNode;
                fseek(drive_image, cur_inode_index, SEEK_SET);
                fread(current_dir, sizeof(struct iNode), 1, drive_image);
            } else if (next_filename != NULL) {
                // If folder throw error
                printf("Error: directory %s doesn't exists.\n", cur_filename);
                return NULL;
            } else {
                // Create file
                had_to_be_created = 1;
                next_dir = malloc(sizeof(struct iNode));
                memcpy(next_dir->filename, cur_filename, 256);
                next_dir->type = type;
                memcpy(next_dir->owner, user, 32);
                time_t raw_time;
                time(&raw_time);
                next_dir->modified_datetime = raw_time;
                next_dir->created_datetime = raw_time;
                next_dir->size = 0;
                next_dir->iNode_parent = cur_inode_index;
                next_dir->continuation_iNode = 0;
                for (int i = 0; i < 15; i++) {
                    next_dir->blocks[i] = 0;
                }

                // Search for empty inode for file and write
                unsigned long inode_index = working_drive->superblock.first_inode;
                struct iNode temp;
                int found_inode = 0;

                for (int inode_counter = 0; inode_counter < working_drive->superblock.inode_count; inode_counter++) {
                    inode_index += sizeof(struct iNode);
                    fseek(drive_image, inode_index, SEEK_SET);
                    fread(&temp, sizeof(struct iNode), 1, drive_image);

                    if (temp.type == EMPTY) {
                        next_dir->index = inode_index;
                        fseek(drive_image, inode_index, SEEK_SET);
                        fwrite(next_dir, sizeof(struct iNode), 1, drive_image);
                        found_inode = 1;
                        break;
                    }
                }

                if (!found_inode) {
                    printf("Error: No inodes available.");
                    return NULL;
                }

                // Search for empty block reference in current directory and write
                int found_reference = 0;
                while (!found_reference) {
                    for (int reference_index = 0; reference_index < 15; reference_index++) {
                        if (current_dir->blocks[reference_index] == 0) {
                            current_dir->blocks[reference_index] = inode_index;
                            found_reference = 1;
                            fseek(drive_image, cur_inode_index, SEEK_SET);
                            fwrite(current_dir, sizeof(struct iNode), 1, drive_image);
                            break;
                        }
                    }

                    if (!found_reference) {
                        if (current_dir->continuation_iNode != 0) {
                            // Check continuation inode
                            cur_inode_index = current_dir->continuation_iNode;
                            fseek(drive_image, cur_inode_index, SEEK_SET);
                            fread(current_dir, sizeof(struct iNode), 1, drive_image);
                        } else {
                            // Search for empty inode for continuation
                            unsigned long continuation_index = working_drive->superblock.first_inode;
                            for (int inode_counter = 0;
                                 inode_counter < working_drive->superblock.inode_count; inode_counter++) {
                                continuation_index += sizeof(struct iNode);
                                fseek(drive_image, continuation_index, SEEK_SET);
                                fread(&temp, sizeof(struct iNode), 1, drive_image);

                                if (temp.type == EMPTY) {
                                    current_dir->continuation_iNode = continuation_index;
                                    fseek(drive_image, cur_inode_index, SEEK_SET);
                                    fwrite(current_dir, sizeof(struct iNode), 1, drive_image);

                                    struct iNode extension_dir = *current_dir;
                                    extension_dir.type = DIRECTORY_EXTENSION;
                                    time_t raw_time;
                                    time(&raw_time);
                                    extension_dir.modified_datetime = raw_time;
                                    extension_dir.created_datetime = raw_time;
                                    extension_dir.size = 0;
                                    extension_dir.continuation_iNode = 0;
                                    extension_dir.iNode_parent = cur_inode_index;
                                    extension_dir.blocks[0] = inode_index;
                                    for (int i = 1; i < 15; i++) {
                                        extension_dir.blocks[i] = 0;
                                    }

                                    fseek(drive_image, continuation_index, SEEK_SET);
                                    fwrite(&extension_dir, sizeof(struct iNode), 1, drive_image);
                                    found_reference = 1;
                                    break;
                                }
                            }
                            if (!found_reference) {
                                printf("Error: No inodes available.");
                                return NULL;
                            }
                        }
                    }
                }
                // Exit main loop
                break;
            }
        }
    }

    if (next_dir->type == DIRECTORY_START && !had_to_be_created) {
        printf("Error: directory already exists with that name.\n");
        return NULL;
    } else {
        return next_dir;
    }
};

struct FileDescriptor *open_file(char *filename) {
    // Return file descriptor
    struct iNode *next_dir = open_inode(filename, FILE_START);
    struct FileDescriptor *file_descriptor = malloc(sizeof(struct FileDescriptor));
    file_descriptor->cursor = 0;
    file_descriptor->drive = working_drive;
    file_descriptor->inode = next_dir;
    return file_descriptor;

};

int create_dir(char *filename) {
    struct iNode *dir = open_inode(filename, DIRECTORY_START);
    if (dir != NULL) {
        return 0;
    } else {
        return 1;
    }
}

// TODO check that all intermediate inodes are directories
int change_directory(char *filename) {
    // Select current directory to search from
    struct iNode *current_dir = malloc(sizeof(struct iNode));
    unsigned long cur_inode_index;
    int using_root;
    if (filename[0] == '/') {
        memcpy(current_dir, &(working_drive->root), sizeof(struct iNode));
        cur_inode_index = working_drive->superblock.first_inode;
        using_root = 1;
    } else {
        cwd_index = 288;
        fseek(drive_image, cwd_index, SEEK_SET);
        fread(current_dir, sizeof(struct iNode), 1, drive_image);
        cur_inode_index = cwd_index;
        using_root = 0;
    }

    // Split path
    char *cur_filename = strtok(filename, "/");
    char *next_filename = strtok(NULL, "/");
    struct iNode *next_dir = malloc(sizeof(struct iNode));
    int found;

    while (cur_filename != NULL) {
        found = 0;
        // Search current filename in current_dir to get next dir
        unsigned long block_index;
        for (int i = 0; i < 15; i++) {
            block_index = current_dir->blocks[i];
            if (block_index != 0) {
                fseek(drive_image, block_index, SEEK_SET);
                fread(next_dir, sizeof(struct iNode), 1, drive_image);
                if (strcmp(next_dir->filename, cur_filename) == 0) {
                    current_dir = next_dir;
                    cur_inode_index = block_index;
                    cur_filename = next_filename;
                    next_filename = strtok(NULL, "/");
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            if (current_dir->continuation_iNode != 0) {
                // Check continuation_inode
                cur_inode_index = current_dir->continuation_iNode;
                fseek(drive_image, cur_inode_index, SEEK_SET);
                fread(current_dir, sizeof(struct iNode), 1, drive_image);
            } else if (next_filename != NULL) {
                // If folder throw error
                printf("Error: directory %s doesn't exists.\n", cur_filename);
                return NULL;
            } else {

                // Exit main loop
                break;
            }
        }
    }

    if (next_dir->type == DIRECTORY_START) {
        cwd_index = cur_inode_index;
        return 0;
    } else {
        return 1;
    }
};

int write_file(struct FileDescriptor *fileDescriptor, char *data, int size) {
    struct iNode *inode = malloc(sizeof(struct iNode));
    memcpy(inode, fileDescriptor->inode, sizeof(struct iNode));
    struct Block *block = malloc(working_drive->superblock.block_size);
    unsigned long block_index = working_drive->superblock.first_block;
    int pointer_index = fileDescriptor->cursor / (working_drive->superblock.block_size - sizeof(enum state_t));
    int offset = fileDescriptor->cursor % (working_drive->superblock.block_size - sizeof(enum state_t));
    int pointer;
    int scanned_for_contiguous_space = 0;

    while (size > 0) {
        if (pointer_index < 15) {
            pointer = inode->blocks[pointer_index];
            if (pointer != 0) {
                // If block is assigned
                block_index = pointer;
                fseek(drive_image, pointer, SEEK_SET);
                fread(block, working_drive->superblock.block_size, 1, drive_image);

            } else {
                // Else assign block
                int found_contiguous_space = 0;
                if (!scanned_for_contiguous_space) {
                    // Search for contiguous space
                    int blocks_needed =
                            (offset + size) / (working_drive->superblock.block_size - sizeof(enum state_t)) +
                            ((offset + size) / (working_drive->superblock.block_size - sizeof(enum state_t)) != 0);
                    unsigned long current_index = block_index;
                    int contiguous_blocks = 0;
                    struct Block *current_block = malloc(working_drive->superblock.block_size);

                    for (int new_index = 0; new_index < working_drive->superblock.block_count; new_index++) {
                        fseek(drive_image, current_index, SEEK_SET);
                        fread(current_block, working_drive->superblock.block_size, 1, drive_image);
                        current_index += working_drive->superblock.block_size;
                        if (current_block->state == EMPTY) {
                            contiguous_blocks += 1;
                            if (contiguous_blocks == blocks_needed) {
                                found_contiguous_space = 1;
                                break;
                            }
                        } else {
                            block_index = current_index;
                            contiguous_blocks = 0;
                        }
                    }
                }

                if (!found_contiguous_space && !scanned_for_contiguous_space) {
                    // Start from the beginning
                    block_index = working_drive->superblock.first_block;
                }

                scanned_for_contiguous_space = 1;

                while (block_index < working_drive->superblock.block_count) {
                    fseek(drive_image, block_index, SEEK_SET);
                    fread(block, working_drive->superblock.block_size, 1, drive_image);

                    if (block->state == EMPTY) {
                        if(inode->type == FILE_START){
                            inode->size += working_drive->superblock.block_size - sizeof(enum state_t);
                        }else {
                            fileDescriptor->inode->size += working_drive->superblock.block_size - sizeof(enum state_t);
                            fseek(drive_image, fileDescriptor->inode->index, SEEK_SET);
                            fwrite(fileDescriptor->inode, sizeof(struct iNode), 1, drive_image);
                        }
                        break;
                    } else {
                        block_index += working_drive->superblock.block_size;
                    }
                }

                inode->blocks[pointer_index] = block_index;
                fseek(drive_image, inode->index, SEEK_SET);
                fwrite(inode, sizeof(struct iNode), 1, drive_image);

                if(inode->type == FILE_START){
                    memcpy(fileDescriptor->inode, inode, sizeof(struct iNode));
                }
            }

            // Write to block
            block->state = USED;
            int amount_written;
            if(working_drive->superblock.block_size - sizeof(enum state_t) - offset < size) {
                amount_written = working_drive->superblock.block_size - sizeof(enum state_t) - offset;
            }else{
                amount_written = size;
            }
            memcpy(block->information + offset, data, amount_written);
            data = data + amount_written;
            size -= amount_written;
            fileDescriptor->cursor += amount_written;
            pointer_index += 1;
            offset = 0;

            fseek(drive_image, block_index, SEEK_SET);
            fwrite(block, working_drive->superblock.block_size, 1, drive_image);

            block_index += working_drive->superblock.block_size;

        } else {
            // Check continuation inode
            int found_inode = 0;
            if (inode->continuation_iNode != 0) {
                fseek(drive_image, inode->continuation_iNode, SEEK_SET);
                fread(inode, sizeof(struct iNode), 1, drive_image);
            } else {
                // Search for empty inode for continuation
                unsigned long continuation_index = working_drive->superblock.first_inode;
                struct iNode temp;
                for (int inode_counter = 0;
                     inode_counter < working_drive->superblock.inode_count; inode_counter++) {
                    continuation_index += sizeof(struct iNode);
                    fseek(drive_image, continuation_index, SEEK_SET);
                    fread(&temp, sizeof(struct iNode), 1, drive_image);

                    if (temp.type == EMPTY) {
                        inode->continuation_iNode = continuation_index;
                        fseek(drive_image, inode->index, SEEK_SET);
                        fwrite(inode, sizeof(struct iNode), 1, drive_image);
                        memcpy(fileDescriptor->inode, inode, sizeof(struct iNode));

                        struct iNode *extension = malloc(sizeof(struct iNode));
                        memcpy(extension, inode, sizeof(struct iNode));
                        extension->index = continuation_index;
                        extension->type = DIRECTORY_EXTENSION;
                        time_t raw_time;
                        time(&raw_time);
                        extension->modified_datetime = raw_time;
                        extension->created_datetime = raw_time;
                        extension->size = 0;
                        extension->continuation_iNode = 0;
                        extension->iNode_parent = inode->index;
                        for (int i = 0; i < 15; i++) {
                            extension->blocks[i] = 0;
                        }

                        fseek(drive_image, continuation_index, SEEK_SET);
                        fwrite(extension, sizeof(struct iNode), 1, drive_image);
                        found_inode = 1;
                        inode = extension;
                        break;
                    }
                }
                if (!found_inode) {
                    printf("Error: No inodes available.");
                    return NULL;
                }
            }
            pointer_index -= 15;
        }
    }

    return 0;
};

char *read_file(struct FileDescriptor *fileDescriptor) {
    struct iNode *inode = malloc(sizeof(struct iNode));
    memcpy(inode, fileDescriptor->inode, sizeof(struct iNode));
    struct Block *block = malloc(working_drive->superblock.block_size);
    int pointer_index = fileDescriptor->cursor / (working_drive->superblock.block_size - sizeof(enum state_t));
    int offset = fileDescriptor->cursor % (working_drive->superblock.block_size - sizeof(enum state_t));
    int pointer;
    int copied = 0;
    int size = inode->size - fileDescriptor->cursor;
    char *data = malloc(size);

    while (copied < size) {
        if (pointer_index < 15) {
            pointer = inode->blocks[pointer_index];
            if (pointer != 0) {
                // If block is assigned
                fseek(drive_image, pointer, SEEK_SET);
                fread(block, working_drive->superblock.block_size, 1, drive_image);
            }
        } else {
            // Load next inode
            if (inode->continuation_iNode != 0) {
                fseek(drive_image, inode->continuation_iNode, SEEK_SET);
                fread(inode, sizeof(struct iNode), 1, drive_image);
                pointer_index -= 15;
            } else {
                printf("Corrupt file, size is bigger than assigned inodes.");
                return NULL;
            }
        }

        int to_be_copied;
        if (working_drive->superblock.block_size - sizeof(enum state_t) < size) {
            to_be_copied = working_drive->superblock.block_size - sizeof(enum state_t);
        } else {
            to_be_copied = size;
        }
        memcpy(data + copied, block->information + offset, to_be_copied);
        copied += to_be_copied;
        pointer_index += 1;
        offset = 0;
    }

    free(inode);
    free(block);
    return data;
};

int list_directories() {
    printf("-------------------\n");
    struct iNode current;
    fseek(drive_image, cwd_index, SEEK_SET);
    fread(&current, sizeof(struct iNode), 1, drive_image);
    printf("Current directory: %s\n", current.filename);

    struct iNode sub_dir;
    while (1) {
        for (int reference_index = 0; reference_index < 15; reference_index++) {
//            printf("Block: %d Index: %d\n", reference_index, current.blocks[reference_index]);
            if (current.blocks[reference_index] != 0) {
                fseek(drive_image, current.blocks[reference_index], SEEK_SET);
                fread(&sub_dir, sizeof(struct iNode), 1, drive_image);
                printf("%s\n", sub_dir.filename);
            }
        }

        if (current.continuation_iNode) {
            fseek(drive_image, current.continuation_iNode, SEEK_SET);
            fread(&current, sizeof(struct iNode), 1, drive_image);
        } else {
            return 0;
        }
    }
}

int seek(struct FileDescriptor *fileDescriptor, int index){
    fileDescriptor->cursor = index;
    return 0;
};