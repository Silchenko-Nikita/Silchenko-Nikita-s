from PyQt4 import QtCore
from OpenGL.GL import *
from  figures import Cube, Color

from random import randint

class Block(Cube):
    WIDTH = 1

    @staticmethod
    def display(center, color: Color, circuit_color=Color.BLACK):
        Cube.display(center, color, circuit_color)


def _clearDoubleArr(arr):
    for i in range(len(arr)):
        for j in range(len(arr[i])):
            arr[i][j] = 0

def _clearTrippleArr(arr):
    for i in range(len(arr)):
        for j in range(len(arr[i])):
            for k in range(len(arr[i][j])):
                arr[i][j][k] = 0


def _coppyTrippleArr(dst, src):
    for i in range(len(src)):
        for j in range(len(src[i])):
            for k in range(len(src[i][j])):
                dst[i][j][k] = src[i][j][k]

class Tetromino:
    TYPES_NUM = 5
    REPR_WIDTH = 4

    _buff_arr = [[0 for p in range(4)] for t in range(4)]# 4 is REPR_WIDTH

    def __init__(self, field):
        self.color = Color.YELLOW
        self._field = field
        self.repr = [[[0 for k in range(self.REPR_WIDTH)] for j in range(self.REPR_WIDTH)] for i in range(self.REPR_WIDTH)]
        self._buff_repr = [[[0 for k in range(self.REPR_WIDTH)] for j in range(self.REPR_WIDTH)] for i in range(self.REPR_WIDTH)]
        self.initialRandomize()

    def initialRandomize(self):
        field = self._field
        self._clearRepr()

        self.xOffset = field.WIDTH // 2 - self.REPR_WIDTH // 2
        self.zOffset = field.HEIGHT // 2 - self.REPR_WIDTH // 2
        self.yOffset = field.ALTITUDE

        rand = randint(0, self.TYPES_NUM - 1)

        if(rand == 0):
            self.color = Color.YELLOW

            self.repr[1][1][1] = 1
            self.repr[1][1][2] = 1
            self.repr[1][2][1] = 1
            self.repr[1][2][2] = 1
            self.repr[2][1][1] = 1
            self.repr[2][1][2] = 1
            self.repr[2][2][1] = 1
            self.repr[2][2][2] = 1

        elif(rand == 1):
            self.color = Color.RED

            self.repr[0][2][1] = 1
            self.repr[1][2][1] = 1
            self.repr[2][2][1] = 1
            self.repr[3][2][1] = 1

        elif(rand == 2):
            self.color = Color.PURPLE

            self.repr[0][2][1] = 1
            self.repr[1][2][1] = 1
            self.repr[2][2][1] = 1
            self.repr[1][2][2] = 1

        elif(rand == 3):
            self.color = Color.BLUE

            self.repr[0][2][1] = 1
            self.repr[1][2][1] = 1
            self.repr[2][2][1] = 1
            self.repr[2][2][2] = 1
            self.repr[2][1][2] = 1
        elif(rand == 4):
            self.color = Color.GREEN

            self.repr[0][2][1] = 1
            self.repr[1][2][1] = 1
            self.repr[2][2][1] = 1
            self.repr[2][1][1] = 1
            self.repr[2][1][2] = 1

    def display(self):
        for i in range(self.REPR_WIDTH):
            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    if self.repr[i][j][k]:
                        blockCenterCoords = (Block.WIDTH * (-self._field.WIDTH/2 + self.xOffset + 0.5 + i),
                                             Block.WIDTH * (self.yOffset + 0.5 + (j - (self.REPR_WIDTH - 1))),
                                             Block.WIDTH * (-self._field.HEIGHT/2 + self.zOffset + 0.5 + k))
                        Block.display(blockCenterCoords, self.color)

    def move(self, x, y, z) -> bool:
        self.xOffset += x
        self.yOffset += y
        self.zOffset += z

        if not self._posIsValid():
            self.xOffset -= x
            self.yOffset -= y
            self.zOffset -= z
            return False

        return True

    def fall(self) -> bool:
        return self.move(0, -1, 0)

    def rotateX_CW(self):
        _clearDoubleArr(self._buff_arr)
        _clearTrippleArr(self._buff_repr)

        for i in range(self.REPR_WIDTH):
            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self._buff_repr[i][k][self.REPR_WIDTH - 1 - j] = self.repr[i][k][self.REPR_WIDTH - 1 - j]
                    self._buff_arr[j][k] = self.repr[i][k][self.REPR_WIDTH - 1 - j]

            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self.repr[i][j][k] = self._buff_arr[j][k]

        if not self._posIsValid():
            _coppyTrippleArr(self.repr, self._buff_repr)
            return False

        return True

    def rotateX_CCW(self):
        _clearDoubleArr(self._buff_arr)
        _clearTrippleArr(self._buff_repr)

        for i in range(self.REPR_WIDTH):
            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self._buff_repr[i][j][k] = self.repr[i][j][k]
                    self._buff_arr[k][self.REPR_WIDTH - 1 - j] = self.repr[i][j][k]

            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self.repr[i][j][k] = self._buff_arr[j][k]

        if not self._posIsValid():
            _coppyTrippleArr(self.repr, self._buff_repr)
            return False

        return True


    def rotateY_CW(self):
        _clearDoubleArr(self._buff_arr)
        _clearTrippleArr(self._buff_repr)

        for i in range(self.REPR_WIDTH):
            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self._buff_repr[self.REPR_WIDTH - 1 - j][i][k] = self.repr[self.REPR_WIDTH - 1 - j][i][k]
                    self._buff_arr[j][k] = self.repr[self.REPR_WIDTH - 1 - j][i][k]

            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self.repr[k][i][j] = self._buff_arr[j][k]

        if not self._posIsValid():
            _coppyTrippleArr(self.repr, self._buff_repr)
            return False

        return True


    def rotateY_CCW(self):
        _clearDoubleArr(self._buff_arr)
        _clearTrippleArr(self._buff_repr)

        for i in range(self.REPR_WIDTH):
            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self._buff_repr[k][i][j] = self.repr[k][i][j]
                    self._buff_arr[k][self.REPR_WIDTH - 1 - j] = self.repr[k][i][j]

            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    self.repr[k][i][j] = self._buff_arr[j][k]

        if not self._posIsValid():
            _coppyTrippleArr(self.repr, self._buff_repr)
            return False

        return True


    def _posIsValid(self):
        field = self._field
        for i in range(self.REPR_WIDTH):
            for j in range(self.REPR_WIDTH):
                for k in range(self.REPR_WIDTH):
                    fieldX = self.xOffset + i
                    fieldY = self.yOffset - self.REPR_WIDTH + j
                    fieldZ = self.zOffset + k
                    if self.repr[i][j][k] and (not field.coordsAreValid(fieldX, fieldY, fieldZ) or
                        (field.coordsAreValid(fieldX, fieldY, fieldZ) and
                                    field.view[fieldX][fieldY][fieldZ] != Color.TRANSPARENT)):
                        return False
        return True


    def _clearRepr(self):
        _clearTrippleArr(self.repr)