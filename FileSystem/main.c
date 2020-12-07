#include <stdio.h>
#include "BootAndReset/DiskManagement.h"
#include "FileManagement/FileDescriptor.h"
#include "FileManagement/FileRW.h"

int main() {
    struct Drive *drive = createDrive("drive.vsfs", 104857600, 4096, 1000, "jassonrm");
    checkIntegrity("drive.vsfs");
    set_working_drive(drive);
    struct FileDescriptor *fd = open_file("/prueba.exe");
    write_file(fd, "Hola mundo!", 12);
    return 0;
}
