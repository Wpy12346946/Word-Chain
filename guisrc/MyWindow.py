from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
import os

from window import Ui_MainWindow
import utils
from core import *


class MyMainWindow(Ui_MainWindow, QMainWindow):
    def __init__(self):
        super(MyMainWindow, self).__init__()
        self.setupUi(self)

        self.output = ""

        self.selectFileButton.clicked.connect(self.chooseFile)
        self.outputButton.clicked.connect(self.outputFile)

        self.allowHeadButton.stateChanged.connect(self.headClick)
        self.allowTailButton.stateChanged.connect(self.tailClick)
        self.allowRejectButton.stateChanged.connect(self.rejectClick)
        self.allowCircleButton.stateChanged.connect(self.circleClick)

        self.allButton.clicked.connect(self.findAll)
        self.wordButton.clicked.connect(self.findMaxWord)
        self.charButton.clicked.connect(self.findMaxChar)

    def chooseFile(self):
        filename, t = QFileDialog.getOpenFileName(self, "打开文件", '', 'Text (*.txt)')
        self.showFileText.setText(filename)

        if os.path.exists(filename):
            with open(filename, 'r') as f:
                content = f.read()
            self.showInputText.setText(content)
            self.output = content
        else:
            self.showInputText.setText("")

    def outputFile(self):
        filename, t = QFileDialog.getSaveFileName(self, '导出结果', 'solution', "Text (*.txt)")
        if filename:
            with open(filename, 'w') as f:
                f.write(self.output)

    # 更新allButton（查询所有链）按钮状态
    def paramClick(self, state, button, textEdit=None):
        if textEdit:
            if state == Qt.Checked:
                textEdit.setDisabled(False)
                textEdit.setText('a')
                textEdit.setFocus()
            else:
                textEdit.setDisabled(True)
                textEdit.setText('')

        if self.allowHeadButton.isChecked() \
                or self.allowTailButton.isChecked() \
                or self.allowRejectButton.isChecked() \
                or self.allowCircleButton.isChecked():
            self.allButton.setDisabled(True)
        else:
            self.allButton.setDisabled(False)

    # 可选参数的交互
    def headClick(self, state):
        self.paramClick(state, self.allowHeadButton, self.headText)

    def tailClick(self, state):
        self.paramClick(state, self.allowTailButton, self.tailText)

    def rejectClick(self, state):
        self.paramClick(state, self.allowRejectButton, self.rejectText)

    def circleClick(self, state):
        self.paramClick(state, self.allowCircleButton)

    def warnMessage(self, message):
        messageBox = QMessageBox(self)
        messageBox.setText("错误提示")
        messageBox.setInformativeText(message)
        messageBox.setIcon(QMessageBox.Critical)
        messageBox.exec_()

    def findAll(self):
        try:
            results, length, times = gen_chains_all(self.showInputText.toPlainText())
            self.timeLabel.setText("运行时间：" + str(round(times, 2)) + "秒")
            self.showOutputText.setText(str(length) + "\n" + results)
        except WordException as e:
            self.warnMessage("运行错误，出错原因：" + str(e))
            self.timeLabel.setText("运行错误")
            self.showOutputText.setText("")

    def findMaxWord(self):
        self.findMaxLen(gen_chain_word)

    def findMaxChar(self):
        self.findMaxLen(gen_chain_char)

    def findMaxLen(self, function):
        # 参数检查
        if self.allowHeadButton.isChecked():
            if not self.headText.text():
                self.warnMessage("请输入h参数")
                return
            elif not self.headText.text().isalpha():
                self.warnMessage("h参数需要为字母")
                return
        if self.allowTailButton.isChecked():
            if not self.tailText.text():
                self.warnMessage("请输入t参数")
                return
            elif not self.tailText.text().isalpha():
                self.warnMessage("t参数需要为字母")
                return
        if self.allowRejectButton.isChecked():
            if not self.rejectText.text():
                self.warnMessage("请输入j参数")
                return
            elif not self.rejectText.text().isalpha():
                self.warnMessage("j参数需要为字母")
                return

                # 参数设置
        head = '\0'
        if self.allowHeadButton.isChecked():
            head = self.headText.text()
        tail = '\0'
        if self.allowTailButton.isChecked():
            tail = self.tailText.text()
        reject = '\0'
        if self.allowRejectButton.isChecked():
            reject = self.rejectText.text()
        enable_loop = self.allowCircleButton.isChecked()

        try:
            results, length, times = function(self.showInputText.toPlainText(), ord(head[0]), ord(tail[0]),
                                              ord(reject[0]), enable_loop)
            self.timeLabel.setText("运行时间：" + str(round(times, 2)) + "秒")
            self.showOutputText.setText(results)
        except WordException as e:
            self.warnMessage("运行错误，出错原因：" + str(e))
            self.timeLabel.setText("运行错误")
            self.showOutputText.setText("")
