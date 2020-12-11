//
// Created by kenneth on 10/12/20.
//

#include <bits/types/FILE.h>
#include "json_creation.h"
struct json_object *
create_json_(char *file_name, char *type, char *owner, char *modify_date, char *created_date, char *size,
             char *inode_owner, char *inode_num, json_object *array){
    json_object * object = json_object_new_object();
    json_object * file_name_json = json_object_new_string(file_name);
    json_object * type_json = json_object_new_string(type);
    json_object * owner_json = json_object_new_string(owner);
    json_object * modify_date_json = json_object_new_string(modify_date);
    json_object * created_date_json = json_object_new_string(created_date);
    json_object * size_json = json_object_new_string(size);
    json_object * inode_owner_json = json_object_new_string(inode_owner);
    json_object * inode_num_json = json_object_new_string(inode_num);
    json_object_object_add(object,"file_name",file_name_json);
    json_object_object_add(object,"type",type_json);
    json_object_object_add(object,"owner",owner_json);
    json_object_object_add(object,"modify_date",modify_date_json);
    json_object_object_add(object,"created_date",created_date_json);
    json_object_object_add(object,"size",size_json);
    json_object_object_add(object,"inode_owner",inode_owner_json);
    json_object_object_add(object,"inode_num",inode_num_json);
    if (array!=NULL){
        json_object_object_add(object,"files",array);
    }
    return object;


}
struct  json_object * create_new_array(){
    return json_object_new_array();
}
void * add_to_array(json_object * array,json_object * file_to_add){
    json_object_array_add(array,file_to_add);
}
void * add_to_already_created(json_object * json_array,json_object * target){
    json_object_object_add(target,"files",json_array);
}
void * write_json(char * filename, json_object * object){
    FILE * file = fopen(filename,"w");
    char * object_txt = json_object_to_json_string(object);
    fwrite(object_txt,1,strlen(object_txt),file);

}