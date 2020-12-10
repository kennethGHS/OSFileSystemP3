from PyQt5.QtWidgets import QLabel


class Label(QLabel):

    def __init__(self,text = None):
        super().__init__(text)

