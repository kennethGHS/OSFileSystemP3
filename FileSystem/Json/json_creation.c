//
// Created by kenneth on 10/12/20.
//

#include <stdio.h>
#include "json_creation.h"

struct json_object *
create_json_(char *file_name, char *type, char *owner, char *modify_date, char *created_date, char *size,
             char *inode_owner, char *inode_num, json_object *array) {
    json_object *object = json_object_new_object();
    json_object *file_name_json = json_object_new_string(file_name);
    json_object *type_json = json_object_new_string(type);
    json_object *owner_json = json_object_new_string(owner);
    json_object *modify_date_json = json_object_new_string(modify_date);
    json_object *created_date_json = json_object_new_string(created_date);
    json_object *size_json = json_object_new_string(size);
    json_object *inode_owner_json = json_object_new_string(inode_owner);
    json_object *inode_num_json = json_object_new_string(inode_num);
    json_object_object_add(object, "file_name", file_name_json);
    json_object_object_add(object, "type", type_json);
    json_object_object_add(object, "owner", owner_json);
    json_object_object_add(object, "modify_date", modify_date_json);
    json_object_object_add(object, "created_date", created_date_json);
    json_object_object_add(object, "size", size_json);
    json_object_object_add(object, "inode_owner", inode_owner_json);
    json_object_object_add(object, "inode_num", inode_num_json);
    if (array != NULL) {
        json_object_object_add(object, "files", array);
    }
    return object;
/**
 * list directories para hacer las busqueda
 * list directories / da el root
 * Retorna lista de inodes
 * al final le pone un inode de tipo empty para saber si se llego al final
 *
 */

}

struct json_object *create_new_array() {
    return json_object_new_array();
}

void *add_to_array(json_object *array, json_object *file_to_add) {
    json_object_array_add(array, file_to_add);
}

void *add_to_already_created(json_object *json_array, json_object *target) {
    json_object_object_add(target, "files", json_array);
}

void *write_json(char *filename, json_object *object) {
    FILE *file = fopen(filename, "w");
    char *object_txt = json_object_to_json_string(object);
    fwrite(object_txt, 1, strlen(object_txt), file);
    fclose(file);

}

void generate_and_run_interface(char *filename) {
    char directory[]="/";
    json_object *root = create_json_("root", "folder", "root", "root", "root", "10000", "1", "2",
                                     parse_directory("/"));
    char * jsons =  json_object_to_json_string(root);
    printf("%s",jsons);
    write_json(filename, root);

    fflush(stdout);
    char cmd[200];
    strcpy(cmd, "python3 ../Json/PythonFiles/main.py ");
    int x = system(strcat(cmd, filename));
    fflush(stdout);
}

struct json_object *parse_directory(char *directory) {
    struct iNode *file_list = list_directories(directory);
    json_object * array = json_object_new_array();
    int i = 0;
    while ((file_list+i)->type != EMPTY){
        json_object * currentFile = NULL;
        char * modify_date = malloc(sizeof(char)*50);
        struct tm* modify_date_tm = localtime(&((file_list+i)->modified_datetime));

        strcpy(modify_date,asctime(modify_date_tm));
        char * created_date = malloc(sizeof(char)*50);
        struct tm* created_date_tm = localtime(&((file_list+i)->modified_datetime));

        strcpy(created_date,asctime(created_date_tm));
        char * inode_owner = malloc(sizeof(char)*9);
        char * inode_num = malloc(sizeof(char)*9);
        char * file_name = (file_list+i)->filename;
        char * owner = (file_list+i)->owner;
        char * size = malloc(sizeof(char )*20);
        sprintf(inode_owner,"%lu",((file_list+i)->iNode_parent));
        sprintf(inode_num,"%lu",((file_list+i)->index));
        sprintf(size,"%lu",((file_list+i)->size));
        if ((file_list+i)->type==DIRECTORY_START){
            char * type = "folder";
            if (strlen(directory)==1){
                //Estoy en root
                char * new_directory[200];
                strcpy(new_directory,directory);
                strcat(new_directory,(file_list+i)->filename);
                json_object * new_array = parse_directory(new_directory);
                currentFile = create_json_(file_name,type,owner,modify_date,created_date,size,inode_owner,inode_num,new_array);
            } else{
                char * new_directory[200];
                strcpy(new_directory,directory);
                strcat(new_directory,"/");
                strcat(new_directory,(file_list+i)->filename);
                json_object * new_array = parse_directory(new_directory);
                currentFile = create_json_(file_name,type,owner,modify_date,created_date,size,inode_owner,inode_num,new_array);
            }
        } else if ((file_list+i)->type==FILE_START){
            char * type = "file";
            currentFile = create_json_(file_name,type,owner,modify_date,created_date,size,inode_owner,inode_num,json_object_new_array());
        }
        add_to_array(array,currentFile);
        i++;
    }
    return array;
}
