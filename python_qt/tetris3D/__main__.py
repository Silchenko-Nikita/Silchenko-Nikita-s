from PyQt4 import QtCore, Qt, QtGui, QtOpenGL
import sys

from OpenGL.GL import *
from game import TetrisGameTread
from tetromino import Block, Tetromino
from figures import Cube
from field import Field


class FrameUpdateThread(QtCore.QThread):

    def __init__(self):
        super().__init__()

    def run(self):
        while True:
            self.msleep(25)
            self.emit(QtCore.SIGNAL("redisplay()"))


class TetrisGameWidget(QtOpenGL.QGLWidget):
    def __init__(self):
        super().__init__()
        self._tetrGame = TetrisGameTread()
        # self._frameUpdateThread = FrameUpdateThread()
        # self.connect(self._frameUpdateThread, QtCore.SIGNAL("redisplay()"), self.glDraw)
        self.connect(self._tetrGame, QtCore.SIGNAL("tetrominoUpdate()"), self.glDraw)
        self._tetrGame.start()
        # self._frameUpdateThread.start()

    def initializeGL(self):
        glClearColor(0.0, 0.0, 0.0, 0.0)
        glEnable(GL_DEPTH_TEST)


    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self._tetrGame.displayAll()
        glFlush()


    def resizeGL(self, width, height):
        side = min(width, height)
        if side <= 0:
            return


        glMatrixMode(GL_PROJECTION)
        glViewport((width - side) // 2, (height - side) // 2, side, side)
        glLoadIdentity()
        glOrtho(-1.5 * self._tetrGame._field.WIDTH * Block.WIDTH, 1.5 * self._tetrGame._field.WIDTH * Block.WIDTH,
                -1.5 * self._tetrGame._field.HEIGHT * Block.WIDTH, 1.5 * self._tetrGame._field.HEIGHT * Block.WIDTH,
                -1.5 * self._tetrGame._field.ALTITUDE * Block.WIDTH, 1.5 * self._tetrGame._field.ALTITUDE * Block.WIDTH)
        # glFrustum(-1.0, +1.0, -1.0, 1.0, 0.5, 60.0)
        glTranslate(0, (-self._tetrGame._field.ALTITUDE) * Block.WIDTH * 0.3, 0)
        glRotate(40, 1, 0, 0)
        glRotate(-30, 0, 1, 0)
        # glTranslate(0, 0, -self._tetrGame._field.HEIGHT * 2.5 * Block.WIDTH)
        glMatrixMode(GL_MODELVIEW)


    def keyPressEvent(self, e):

        rotateAngle = 5
        key = e.key()
        if key == QtCore.Qt.Key_Q:
            self._tetrGame._field.rotate(rotateAngle)
        elif key == QtCore.Qt.Key_E:
            self._tetrGame._field.rotate(-rotateAngle)

        elif key == QtCore.Qt.Key_A:
            self._tetrGame._fallingTetromino.rotateY_CCW()
        elif key == QtCore.Qt.Key_D:
            self._tetrGame._fallingTetromino.rotateY_CW()
        elif key == QtCore.Qt.Key_W:
            self._tetrGame._fallingTetromino.rotateX_CCW()
        elif key == QtCore.Qt.Key_S:
            self._tetrGame._fallingTetromino.rotateX_CW()

        elif key == QtCore.Qt.Key_J:
            self._tetrGame._fallingTetromino.move(-1, 0, 0)
        elif key == QtCore.Qt.Key_L:
            self._tetrGame._fallingTetromino.move(1, 0, 0)
        elif key == QtCore.Qt.Key_I:
            self._tetrGame._fallingTetromino.move(0, 0, -1)
        elif key == QtCore.Qt.Key_K:
            self._tetrGame._fallingTetromino.move(0, 0, 1)


        self.glDraw()


class MainWindow(QtGui.QMainWindow):

    def __init__(self):
        super().__init__()
        tetrisField = TetrisGameWidget()
        self.resize(600, 600)
        self.setCentralWidget(tetrisField)


if __name__ == "__main__":
    app = Qt.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    exit(app.exec_())
