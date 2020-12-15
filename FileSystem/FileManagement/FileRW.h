//
// Created by kenneth on 5/12/20.
//

#ifndef FILESYSTEM_FILERW_H
#define FILESYSTEM_FILERW_H

#include "FileDescriptor.h"
#include "Inode.h"

int set_working_drive(struct Drive *drive);

int set_current_user(char username[32]);

static struct iNode *open_inode(char *filename, enum type_t type);

static int get_inode_index(char *filename);

struct FileDescriptor *open_file(char *filename);

int create_dir(char *filename);

int change_directory(char *filename);

int write_file(struct FileDescriptor *fileDescriptor, char *data, int size);

char *read_file(struct FileDescriptor *fileDescriptor);

struct iNode *list_directories(char *filename);

int seek(struct FileDescriptor *fileDescriptor, int index);

int delete_fd(struct FileDescriptor *fileDescriptor);

int delete_(char *filename);

static int delete_inode_reference(struct iNode *inode);

static int delete_inode(unsigned long index);

static int delete_block(unsigned long index);

int rename_file(char *filename, char *new_name);

int change_owner(char *filename, char *new_owner);

struct iNode *get_attributes(char *filename);

int set_read_only(char *filename, int state);

#endif //FILESYSTEM_FILERW_H
