from PyQt5 import QtCore
from PyQt5.QtWidgets import QLineEdit


class DirText(QLineEdit):
    def __init__(self, window):
        self.main_window_parent = window
        super().__init__()
        self.setText("/")


    def keyPressEvent(self, QKeyEvent):
        print("xd")
        if QKeyEvent.key() == QtCore.Qt.Key_Enter or QKeyEvent.key() == QtCore.Qt.Key_Return:
            self.main_window_parent.check_file_and_set(self.text())
            print("lol")
        else:
            super().keyPressEvent(QKeyEvent)

