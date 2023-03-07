import sys

from PyQt5.QtWidgets import QApplication
from MyWindow import MyMainWindow

if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainWindow = MyMainWindow()
    mainWindow.show()
    app.exec_()
