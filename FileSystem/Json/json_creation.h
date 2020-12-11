//
// Created by kenneth on 10/12/20.
//

#ifndef FILESYSTEM_JSON_CREATION_H
#define FILESYSTEM_JSON_CREATION_H

#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct json_object *
create_json_(char *file_name, char *type, char *owner, char *modify_date, char *created_date, char *size,
             char *inode_owner, char *inode_num, json_object *array);
struct  json_object * create_new_array();
void * add_to_array(json_object * array,json_object * file_to_add);
void * add_to_already_created(json_object * json_array,json_object * target);
void * write_json(char * filename, json_object * object);
#endif //FILESYSTEM_JSON_CREATION_H
