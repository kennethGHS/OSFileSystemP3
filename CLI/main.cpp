#include "mainwindow.h"
#include <QApplication>

extern "C" {
#include "../FileSystem/BootAndReset/DiskManagement.h"
#include "../FileSystem/FileManagement/FileDescriptor.h"
#include "../FileSystem/FileManagement/FileRW.h"
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    struct Drive *drive = createDrive("drive.vsfs", 104857600, 64, 1000, "jassonrm");
    checkIntegrity("drive.vsfs");
    set_working_drive(drive);
    set_current_user("jassonrm");

//    struct FileDescriptor *fd;
//    char number[4];
//    char name[32];
//    for (int i = 0; i < 10; i++) {
//        snprintf(number, sizeof(number), "%d", i);
//        printf("%s\n", number);
//        strcpy(name, number);
//        strcat(name, "-file");
//        open_file(name);
//        strcpy(name, number);
//        strcat(name, "-dir");
//        create_dir(name);
//    }
//    list_directories(NULL);
//    char filename[] = "/1-dir/another_test";
//    create_dir(filename);
//    create_dir("/1-file/another_test");
//    list_directories("1-dir");
//    change_directory("2-file");
//    change_directory("1-dir");
//    change_directory("2-file");
//    create_dir("prueba1");
//    create_dir("prueba2");
//    open_file("prueba3");
//    fd = open_file("prueba4");
//    list_directories(NULL);

    MainWindow w;
    w.show();
    return a.exec();
}
