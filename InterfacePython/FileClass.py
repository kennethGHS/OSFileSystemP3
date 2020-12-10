from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtWidgets import QLabel, QHBoxLayout

from LabelFile import Label


class Inode:
    def __init__(self, file_name, type, label_interface, owner, modify_date, inode_owner, created_date, size,
                 file_inside, inode_num):
        super().__init__()
        self.inode_num = inode_num
        self.file_name = file_name
        self.type = type
        self.label_interface = label_interface
        self.owner = owner
        self.modify_date = modify_date
        self.created_date = created_date
        self.size = size
        self.inode_owner = inode_owner
        self.file_inside = file_inside
        self.label = Label()
        if self.type == "folder":
            image = QImage()
            image.load('folder.png')
            pixmap = QPixmap.fromImage(image)
            self.label.setPixmap(pixmap)
        else:
            image = QImage()
            image.load('text.png')
            pixmap = QPixmap.fromImage(image)
            self.label.setPixmap(pixmap)

        self.label_data = Label(
            f'file_name:{self.file_name},  type:{self.type} \n owner: {self.owner}  '
            f'modify_date: {self.modify_date}   created_date: {self.created_date},   size: {self.size}')


    def print_attributes(self):
        print(f'inode num:{self.inode_num},  file_name:{self.file_name},  type:{self.type}  \n owner: {self.owner}  '
              f'modify_date: {self.modify_date}   created_date: {self.created_date},   size: {self.size}  '
              f'inode_owner: {self.inode_owner}')
        for files in self.file_inside:
            files.print_attributes()

    def reset_labels(self):
        if self.type == "folder":
            image = QImage()
            image.load('folder.png')
            pixmap = QPixmap.fromImage(image)
            self.label.setPixmap(pixmap)
        else:
            image = QImage()
            image.load('text.png')
            pixmap = QPixmap.fromImage(image)
            self.label.setPixmap(pixmap)

        self.label_data = QLabel(
            f'file_name:{self.file_name},type:{self.type} \n owner: {self.owner}'
            f'modify_date: {self.modify_date} created_date: {self.created_date}, size: {self.size}')
