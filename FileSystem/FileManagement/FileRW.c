//
// Created by Jasson Rodríguez Méndez on 6/12/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Inode.h"
#include "../BootAndReset/Drive.h"
#include "FileDescriptor.h"

struct Drive *working_drive;
struct iNode *cur_working_dir;
int cwd_index;
FILE *drive_image;
char user[32];

int set_working_drive(struct Drive *drive) {
    working_drive = drive;
    cur_working_dir = &(drive->root);
    cwd_index = working_drive->superblock.first_inode;
    drive_image = fopen(drive->superblock.filename, "rb");
    if (drive_image == NULL){
        printf("Error: Can't open drive.");
        return 1;
    }
    return 0;
};

int set_current_user(char username[32]) {
    memcpy(user, username, 32);
    return 0;
}

struct FileDescriptor *open_file(char *filename) {
    // Create file descriptor
    struct FileDescriptor *file_descriptor = malloc(sizeof(struct FileDescriptor));
    file_descriptor->cursor = 0;
    file_descriptor->drive = working_drive;

    // Select current directory to search from
    struct iNode *current_dir;
    int cur_inode_index;
    if (filename[0] == '/') {
        current_dir = &(working_drive->root);
        cur_inode_index = working_drive->superblock.first_inode;
    } else {
        current_dir = cur_working_dir;
        cur_inode_index = cwd_index;
    }

    // Split path
    char *cur_filename = strtok(filename, "/");
    char *next_filename = strtok(NULL, "/");
    struct iNode *next_dir;
    int found;

    while (cur_filename != NULL) {
        found = 0;
        // Search current filename in current_dir to get next dir
        int block_index;
        for (int i = 0; i < 15; i++) {
            block_index = current_dir->blocks[i];
            if(block_index != 0) {
                fseek(drive_image, block_index, SEEK_SET);
                fread(next_dir, sizeof(struct iNode), 1, drive_image);
                if (strcmp(next_dir->filename, cur_filename) == 0) {
                    current_dir = next_dir;
                    cur_inode_index = current_dir->blocks[i];
                    cur_filename = next_filename;
                    next_filename = strtok(NULL, "/");
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            if (current_dir->continuation_iNode) {
                // Check continuation_inode
                cur_inode_index = current_dir->continuation_iNode;
                fseek(drive_image, cur_inode_index, SEEK_SET);
                fread(current_dir, sizeof(struct iNode), 1, drive_image);
            } else if (next_filename != NULL) {
                // If folder throw error
                printf("Error: directory %s doesn't exists.", next_filename);
                return NULL;
            } else {
                // Create file
                memcpy(next_dir->filename, cur_filename, 256);
                next_dir->type = FILE_START;
                memcpy(next_dir->owner, user, 32);
                time_t raw_time;
                time(&raw_time);
                next_dir->modified_datetime = raw_time;
                next_dir->created_datetime = raw_time;
                next_dir->size = 0;
                next_dir->iNode_parent = cur_inode_index;
                next_dir->continuation_iNode = 0;
                memset(next_dir->blocks, 0, 15);

                // Search for empty inode for file and write
                int inode_index = working_drive->superblock.first_inode;
                fseek(drive_image, inode_index, SEEK_SET);

                struct iNode temp;
                int found_inode = 0;

                for (int inode_counter = 0; inode_counter < working_drive->superblock.inode_count; inode_counter++) {
                    fseek(drive_image, sizeof(struct iNode), SEEK_CUR);
                    inode_index += sizeof(struct iNode);
                    fread(&temp, sizeof(struct iNode), 1, drive_image);

                    if (temp.type == EMPTY){
                        fwrite(next_dir, sizeof(struct iNode), 1, drive_image);
                        found_inode = 1;
                        break;
                    }
                }

                if(!found_inode){
                    printf("Error: No inodes available.");
                    return NULL;
                }

                // Search for empty block reference in current directory and write
                int found_reference = 0;
                while (!found_reference){
                    for(int reference_index = 0; reference_index < 15; reference_index++){
                        if(current_dir->blocks[reference_index] == 0){
                            current_dir->blocks[reference_index] = inode_index;
                            found_reference = 1;
                            break;
                        }
                    }

                    if(!found_reference){
                        if(current_dir->continuation_iNode){
                            // Check continuation inode
                            cur_inode_index = current_dir->continuation_iNode;
                            fseek(drive_image, cur_inode_index, SEEK_SET);
                            fread(current_dir, sizeof(struct iNode), 1, drive_image);
                        }else{
                            // Search for empty inode for continuation
                            int continuation_index = working_drive->superblock.first_inode;
                            for (int inode_counter = 0; inode_counter < working_drive->superblock.inode_count; inode_counter++) {
                                fseek(drive_image, sizeof(struct iNode), SEEK_CUR);
                                continuation_index += sizeof(struct iNode);
                                fread(&temp, sizeof(struct iNode), 1, drive_image);

                                if (temp.type == EMPTY){
                                    current_dir->continuation_iNode = continuation_index;
                                    fseek(drive_image, cur_inode_index, SEEK_SET);
                                    fwrite(current_dir, sizeof(struct iNode), 1, drive_image);
                                    
                                    current_dir->type = DIRECTORY_EXTENSION;
                                    time_t raw_time;
                                    time(&raw_time);
                                    current_dir->modified_datetime = raw_time;
                                    current_dir->created_datetime = raw_time;
                                    current_dir->size = 0;
                                    current_dir->continuation_iNode = 0;
                                    current_dir->iNode_parent = cur_inode_index;
                                    current_dir->blocks[0] = inode_index;
                                    memset(current_dir->blocks, 0, 15);

                                    fseek(drive_image, continuation_index, SEEK_SET);
                                    fwrite(current_dir, sizeof(struct iNode), 1, drive_image);
                                    found_inode = 1;
                                    break;
                                }
                            }

                            if(!found_inode){
                                printf("Error: No inodes available.");
                                return NULL;
                            }
                        }
                    }
                }
            }
        }
    }

    if (next_dir->type == DIRECTORY_START) {
        printf("Error: can't open directory.");
        return NULL;
    } else {
        file_descriptor->inode = next_dir;
        return file_descriptor;
    }
};

int write_file(struct FileDescriptor *fileDescriptor, char *data, int size) {
    return -1;
};