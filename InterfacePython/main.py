# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
from time import sleep

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPalette
from PyQt5.QtWidgets import QApplication, QPushButton, QStackedLayout, QLabel, QHBoxLayout, QLineEdit

from JsonParser import parse_json_file
from MainWindowClass import MainWindow
from TextInputDir import DirText


def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    app = QApplication([])
    app2 = MainWindow("xd")
    file = parse_json_file("txt.json")
    app2.root = file
    app2.set_directory_interface(app2.root,["/"])
    file.print_attributes()
    # test = QHBoxLayout()
    # var = QLabel("lol")
    # test2 = QHBoxLayout()
    # var2 = QLabel("lol2")
    # test.addWidget(var)
    # test2.addWidget(var2)
    #
    # app2.addToVbox(test)
    # app2.addToVbox(test2)
    # test3 = QHBoxLayout()
    # var3 = QLabel("lol3")
    # line = DirText(app2)
    # test.addWidget(line)
    #
    # test3.addWidget(var3)
    # app2.addToVbox(test3)
    # app3 = app2
    app.exec_()
    # string = ",asf,asfa,sf,asf,asf,"

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
