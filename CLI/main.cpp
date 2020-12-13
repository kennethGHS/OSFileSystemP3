#include "mainwindow.h"
#include <QApplication>

extern "C" {
#include "../FileSystem/BootAndReset/DiskManagement.h"
#include "../FileSystem/FileManagement/FileDescriptor.h"
#include "../FileSystem/FileManagement/FileRW.h"
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    struct Drive *drive = createDrive("drive.vsfs", 104857600, 64, 1000, "root");
    checkIntegrity("drive.vsfs");
    set_working_drive(drive);
    set_current_user("root");
    MainWindow w;
    w.show();
    return a.exec();
}
