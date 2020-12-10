from PyQt5.QtCore import Qt
from PyQt5.QtGui import QImage, QIcon
from PyQt5.QtWidgets import QLabel, QVBoxLayout, QWidget, QScrollArea, QMainWindow, QStackedLayout, QHBoxLayout, \
    QPushButton

from TextInputDir import DirText


class MainWindow(QMainWindow):

    def __init__(self, root):
        super().__init__()
        self.scroll = QScrollArea()  # Scroll Area which contains the widgets, set as the centralWidget
        self.widget = QWidget()  # Widget that contains the collection of Vertical Box
        self.vbox = QVBoxLayout()  # The Vertical Box that contains the Horizontal Boxes of  labels and buttons
        self.root = root
        self.directory_text = DirText(self)
        self.hbox = QHBoxLayout()
        self.back_button = QPushButton()
        self.back_button.clicked.connect(lambda: self.execute_prev_string())
        self.back_button.setStyleSheet("background-color: lightblue;")
        box_layout = QHBoxLayout()
        box_layout.addWidget(self.back_button)
        box_layout.addWidget(self.directory_text)


        self.back_button.setIcon(QIcon("arrow.png"))
        self.vbox.addLayout(box_layout)
        self.setStyleSheet("background-color: white;")
        self.current_dir = "/"
        self.directory_list = ["/"]
        self.initUI(root)

    def initUI(self, root):

        self.widget.setLayout(self.vbox)
        # Scroll Area Properties
        self.scroll.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        # self.scroll.setHorizontalScrollBarPolicy()
        self.scroll.setWidgetResizable(True)
        self.scroll.setWidget(self.widget)
        self.setCentralWidget(self.scroll)
        self.setGeometry(600, 100, 1000, 900)
        self.setWindowTitle('Scroll Area Demonstration')
        self.show()

        return

    def resetQbox(self):
        for i in reversed(range(self.vbox.count())):
            if i == 0:
                continue
            if isinstance(self.vbox.itemAt(i), QHBoxLayout):
                for j in reversed(range(self.vbox.itemAt(i).count())):
                    self.vbox.itemAt(i).itemAt(j).widget().setParent(None)
                self.vbox.removeItem(self.vbox.itemAt(i))
                continue
            self.vbox.itemAt(i).widget().setParent(None)

    def addToVbox(self, item):
        self.vbox.addLayout(item)
        self.vbox.setAlignment(Qt.AlignTop)

    # def mousePressEvent(self, QMouseEvent):
    #     self.resetQbox()

    def mouseDoubleClickEvent(self, QMouseEvent):
        print("xd doubble baby")

    def check_file_and_set(self, path):
        if path == "":
            self.directory_text.setText(self.current_dir)
        if path[0] == "/" and len(path) == 1:
            self.set_directory_interface(self.root, ["/"])

        string_list = path.split("/")[1:]
        if self.root is not None:
            if path == "/":
                self.set_directory_interface(self.root, ["/"])
                return
            folder = self.check_directory(self.root.file_inside, string_list)
            # ocupo añadir cosas aqui
            if folder == 1 or folder == 2:
                return
            self.set_directory_interface(folder, string_list)

    def check_directory(self, file_list, list_strings):
        file_to_return = None
        for name in list_strings:
            file_to_return = self.get_file_inside(name, file_list)
            if file_to_return == 1:
                pass
            elif file_to_return == 2:
                pass
            else:
                file_list = file_to_return.file_inside
        return file_to_return

    def get_file_inside(self, file_name, file_list):
        for file in file_list:
            if file.file_name == file_name:
                if file.type == "folder":
                    return file
                else:
                    return 1
        return 2

    def set_directory_interface(self, file, string_list):
        self.resetQbox()
        i = 0
        box_layout = QHBoxLayout()

        for file in file.file_inside:

            if i % 5 == 0:
                box_layout = QHBoxLayout()
                box_layout.setAlignment(Qt.AlignLeft)
                self.addToVbox(box_layout)
            i += 1
            file.reset_labels(self)
            box_layout.addWidget(file.label)
            box_layout.addWidget(file.label_data)
        # if i%2==0:
        #     print("Entre")
        #     var =QLabel()
        #     var2 =QLabel()
        #     box_layout.addWidget(var)
        #     box_layout.addWidget(var2)

        string_text = "/"
        for string in string_list:
            string_text += string + "/"
        if string_text == "///":
            self.directory_list.append(self.current_dir)
            self.current_dir = "/"
            self.directory_text.setText("/")
            return

        self.directory_list.append(self.current_dir)
        self.current_dir = string_text[:len(string_text) - 1]
        self.directory_text.setText(string_text)

    def execute_prev_string(self):
        self.check_file_and_set(self.directory_list[len(self.directory_list) - 1])
        print("Aqui")
        if len(self.directory_list) == 2:
            self.directory_list.pop(len(self.directory_list) - 1)
            return
        if len(self.directory_list) != 1:
            self.directory_list.pop(len(self.directory_list) - 1)
            self.directory_list.pop(len(self.directory_list) - 1)
            return
