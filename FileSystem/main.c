#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BootAndReset/DiskManagement.h"
#include "FileManagement/FileDescriptor.h"
#include "FileManagement/FileRW.h"

int main() {
    // Create drive and print info
    struct Drive *drive = createDrive("drive.vsfs", 104857600, 64, 1000, "jassonrm");
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
        open_file(name);
        strcpy(name, number);
        strcat(name, "-dir");
        create_dir(name);
    }
    list_directories(NULL);
    create_dir("/1-dir/another_test");
    create_dir("/1-file/another_test");
    list_directories("1-dir");
    change_directory("2-file");
    change_directory("1-dir");
    change_directory("2-file");
    create_dir("prueba1");
    create_dir("prueba2");
    open_file("prueba3");
    fd = open_file("prueba4");
    list_directories(NULL);

    char string[145];
    for(int i = 0; i < 10; i++) {
        itoa(i, number, 10);
        strcpy(string, number);
        strcat(string, "-Hola mundo! Esto es una prueba muy larga y necesito saber que tanto funciona la funcion de write file asi que estoy haciendo esto bien largo. ");
        write_file(fd, string, 143);
    }

    write_file(fd, "Prueba!!!", 10);
    seek(fd, 0);
    printf("Lectura: |%s|\n", read_file(fd));
    delete_fd(fd);
    list_directories(NULL);
    change_directory("..");
    list_directories(NULL);
    delete("/1-dir/prueba2");
    list_directories("/1-dir");
    delete("1-dir");
    list_directories(NULL);
    return 0;
}
