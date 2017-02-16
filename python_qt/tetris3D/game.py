from PyQt4 import QtCore, Qt, QtGui, QtOpenGL
from tetromino import Block, Tetromino
from field import Field



class TetrisGameTread(QtCore.QThread):
    MAX_LEVEL = 9


    def __init__(self):
        super().__init__()
        self._level = 1
        self._field = Field()
        self._fallingTetromino = Tetromino(self._field)


    def _timeDelay(self):
        return 1000 - self._level * 50


    def incrLevel(self):
        self._level += 1
        if self._level > self.MAX_LEVEL:
            self._level = self.MAX_LEVEL


    def displayAll(self):
        self._field.display()
        self._fallingTetromino.display()


    def run(self):
        while True:
            self.msleep(self._timeDelay())
            if not self._fallingTetromino.fall():
                self._field.appendTetromino(self._fallingTetromino)
                self._fallingTetromino.initialRandomize()
                self._field.update()

            self.emit(QtCore.SIGNAL("tetrominoUpdate()"))