#include <stdio.h>
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
    struct FileDescriptor *fd = open_file("/prueba.exe");
    list_directories();
//    write_file(fd, "Hola mundo!", 12);
    return 0;
}
