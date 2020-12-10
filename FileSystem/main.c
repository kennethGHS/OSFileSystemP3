#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BootAndReset/DiskManagement.h"
#include "FileManagement/FileDescriptor.h"
#include "FileManagement/FileRW.h"

int main() {
    // Create drive and print info
    struct Drive *drive = createDrive("drive.vsfs", 104857600, 4096, 1000, "jassonrm");
    checkIntegrity("drive.vsfs");

    // Setup drive
    set_working_drive(drive);
    set_current_user("jassonrm");

    // Open non existent file and create directories
    struct FileDescriptor *fd;
    char number[4];
    char name[32];
    for (int i = 0; i < 10; i++) {
        itoa(i, number, 10);
        strcpy(name, number);
        strcat(name, "-file");
        fd = open_file(name);
        strcpy(name, number);
        strcat(name, "-dir");
        create_dir(name);
    }
    list_directories();
    create_dir("/1-dir/another_test");
    change_directory("1-dir");
    create_dir("prueba1");
    create_dir("prueba2");
    open_file("prueba3");
    open_file("prueba4");
    list_directories();
//    write_file(fd, "Hola mundo!", 12);
    return 0;
}
