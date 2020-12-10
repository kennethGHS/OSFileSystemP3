from PyQt5.QtCore import Qt
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
        self.back_button.clicked.connect(lambda : self.execute_prev_string())
        self.setStyleSheet("background-color: white;")
        self.current_dir = "/"
        self.directory_list = ["/"]
        self.initUI(root)

    def initUI(self, root):

        self.widget.setLayout(self.vbox)
        # Scroll Area Properties
        self.scroll.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        self.scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
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

    def mousePressEvent(self, QMouseEvent):
        self.resetQbox()

    def mouseDoubleClickEvent(self, QMouseEvent):
        print("xd doubble baby")

    def check_file_and_set(self, path):
        string_list = path.split("/")[1:]
        if self.root is not None:
            folder = self.check_directory(self.root.file_inside, string_list)
            #ocupo añadir cosas aqui
            self.set_directory_interface(folder, string_list)
            self.directory_list.append(path)

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
        for file in file.file_inside:
            file.reset_labels()
            box_layout = QHBoxLayout()
            box_layout.addWidget(file.label)
            box_layout.addWidget(file.label_data)
            self.addToVbox(box_layout)
        string_text = "/"
        for string in string_list:
            string_text += string + "/"
        self.directory_text.setText(string_text)

    def execute_prev_string(self):
        pass

