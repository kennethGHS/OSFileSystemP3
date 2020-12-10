from PyQt5.QtGui import QFont
from PyQt5.QtWidgets import QLabel


class Label(QLabel):

    def __init__(self,text = None):
        super().__init__(text)
        self.setFont(QFont("Times",9
                           ))
        self.companion = None
        self.parentWindow = None
        self.parent = None


    def enterEvent(self, QEvent) :
        self.setStyleSheet("background-color: lightblue;")
        self.companion.setStyleSheet("background-color: lightblue;")

    def leaveEvent(self, QEvent) :
        self.setStyleSheet("background-color: white;")
        self.companion.setStyleSheet("background-color: white;")

    def mousePressEvent(self, QMouseEvent):
        self.parentWindow.check_file_and_set( self.parentWindow.directory_text.text()+self.parent.file_name)

