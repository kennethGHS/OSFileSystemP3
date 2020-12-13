import json

from FileClass import Inode


def get_file_from_data(data):
    if not (isinstance(data,dict)):
        return
    file_name = data["file_name"]
    type = data['type']
    owner = data['owner']
    modify_date = data['modify_date']
    created_date = data['created_date']
    size = data['size']
    inode_owner = data['inode_owner']
    inode_num = data['inode_num']
    files = data['files']
    files_to_add = []
    for file in files:
        files_to_add.append(get_file_from_data(file))
    inode = Inode(file_name, type, None, owner, modify_date, inode_owner, created_date, size, files_to_add,inode_num)
    return inode


def parse_json_file(filename):
    print("Current dir:" + filename)
    file = open(filename, "r")
    data = json.load(file)
    print(type(data))
    return get_file_from_data(data)
