#include <stdio.h>
#include <stdlib.h>
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

    // Open non existent file
    struct FileDescriptor *fd;
    char name[32];
    for(int i = 0; i < 20; i++) {
        itoa(i, name, 10);
        fd = open_file(name);
    }
    list_directories();
//    write_file(fd, "Hola mundo!", 12);
    return 0;
}
