#include <stdio.h>
#include "BootAndReset/DiskManagement.h"

int main() {
    createDrive("drive.vsfs", 104857600, 4096, 1000, "jassonrm");
    checkIntegrity("drive.vsfs");
    return 0;
}
