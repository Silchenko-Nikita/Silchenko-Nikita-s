from PyQt4 import QtCore, Qt, QtGui, QtOpenGL
from OpenGL.GL import *

from tetromino import Block, Tetromino
from camera import Camera
from figures import Color, Cube

class Field:
    WIDTH = 10
    HEIGHT = 10
    ALTITUDE = 20

    def __init__(self):
        self._camera = Camera()
        self.view = [[[ Color.TRANSPARENT for k in range(self.HEIGHT)] for j in range(self.ALTITUDE)] for i in range(self.WIDTH)]

    def display(self):
        glLineWidth(0.25)

        # glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        # glBegin(GL_QUADS)
        #
        # for i in range(1, self.ALTITUDE + 1):
        #     glVertex(-(self.WIDTH / 2) * Block.WIDTH, i * Block.WIDTH, -(self.HEIGHT / 2) * Block.WIDTH)
        #     glVertex(-(self.WIDTH / 2) * Block.WIDTH, i * Block.WIDTH, (self.HEIGHT / 2) * Block.WIDTH)
        #     glVertex((self.WIDTH / 2) * Block.WIDTH, i * Block.WIDTH, (self.HEIGHT / 2) * Block.WIDTH)
        #     glVertex((self.WIDTH / 2) * Block.WIDTH, i * Block.WIDTH, -(self.HEIGHT / 2) * Block.WIDTH)
        #
        # glEnd()
        # glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        # glDisable(GL_CULL_FACE)

        glEnable(GL_CULL_FACE)
        glCullFace(GL_FRONT)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glBegin(GL_QUADS)

        # glVertex(-(self.WIDTH / 2) * Block.WIDTH, 1  * Block.WIDTH, (-self.HEIGHT / 2) * Block.WIDTH)
        # glVertex(-(self.WIDTH / 2) * Block.WIDTH, 1  * Block.WIDTH, (1 - self.HEIGHT / 2) * Block.WIDTH)
        # glVertex(-(self.WIDTH / 2) * Block.WIDTH, 0, ( 1 - self.HEIGHT / 2) * Block.WIDTH)
        # glVertex(-(self.WIDTH / 2) * Block.WIDTH, 0, (- self.HEIGHT / 2) * Block.WIDTH)

        for i in range(1, self.ALTITUDE + 1, 2):
            for j in range(0, self.HEIGHT):
                glColor(255, 0, 0)
                glVertex(-(self.WIDTH / 2) * Block.WIDTH, (i + j % 2) * Block.WIDTH, ((j + 1)  - self.HEIGHT/2) * Block.WIDTH)
                glVertex(-(self.WIDTH / 2) * Block.WIDTH, (i + j % 2) * Block.WIDTH, (j - self.HEIGHT/2) * Block.WIDTH)
                glVertex(-(self.WIDTH / 2) * Block.WIDTH, (i - 1 + j % 2) * Block.WIDTH, (j - self.HEIGHT/2)  * Block.WIDTH)
                glVertex(-(self.WIDTH / 2) * Block.WIDTH, (i - 1 + j % 2) * Block.WIDTH, ((j + 1) - self.HEIGHT/2) * Block.WIDTH)

                glColor(0, 255, 0)
                glVertex((self.WIDTH / 2) * Block.WIDTH, (i + j % 2) * Block.WIDTH, (j - self.HEIGHT/2) * Block.WIDTH)
                glVertex((self.WIDTH / 2) * Block.WIDTH, (i + j % 2) * Block.WIDTH, ((j + 1) - self.HEIGHT/2) * Block.WIDTH)
                glVertex((self.WIDTH / 2) * Block.WIDTH, (i - 1 + j % 2) * Block.WIDTH, ((j + 1) - self.HEIGHT/2)  * Block.WIDTH)
                glVertex((self.WIDTH / 2) * Block.WIDTH, (i - 1 + j % 2) * Block.WIDTH, (j - self.HEIGHT/2) * Block.WIDTH)

        for i in range(1, self.ALTITUDE + 1, 2):
            for j in range(0, self.WIDTH):
                glColor(0, 0, 255)
                glVertex((j - self.WIDTH/2) * Block.WIDTH, ( i + (j + 1) % 2 ) * Block.WIDTH, -(self.HEIGHT / 2) * Block.WIDTH)
                glVertex(((j + 1) - self.WIDTH/2)  * Block.WIDTH, (i + (j + 1) % 2) * Block.WIDTH, -(self.HEIGHT / 2) * Block.WIDTH)
                glVertex(((j + 1) - self.WIDTH/2)  * Block.WIDTH, (i - 1 + (j + 1) % 2) * Block.WIDTH, -(self.HEIGHT / 2) * Block.WIDTH)
                glVertex((j - self.WIDTH/2) * Block.WIDTH, (i - 1 + (j + 1) % 2) * Block.WIDTH, -(self.HEIGHT / 2) * Block.WIDTH)

                glColor(255, 255, 0)
                glVertex(((j + 1) - self.WIDTH/2) * Block.WIDTH, (i + (j + 1) % 2) * Block.WIDTH, (self.HEIGHT / 2) * Block.WIDTH)
                glVertex((j - self.WIDTH/2)  * Block.WIDTH, (i + (j + 1) % 2) * Block.WIDTH, (self.HEIGHT / 2) * Block.WIDTH)
                glVertex((j - self.WIDTH/2)  * Block.WIDTH, (i - 1 + (j + 1) % 2) * Block.WIDTH, (self.HEIGHT / 2) * Block.WIDTH)
                glVertex(((j + 1) - self.WIDTH/2) * Block.WIDTH, (i - 1 + (j + 1) % 2) * Block.WIDTH, (self.HEIGHT / 2) * Block.WIDTH)

        # frames
        glColor(255, 0, 0)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, self.HEIGHT/2 * Block.WIDTH)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, -self.HEIGHT/2 * Block.WIDTH)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, 0, -self.HEIGHT/2 * Block.WIDTH)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, 0, self.HEIGHT/2 * Block.WIDTH)

        glColor(0, 255, 0)
        glVertex((self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, -self.HEIGHT/2 * Block.WIDTH)
        glVertex((self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, self.HEIGHT/2 * Block.WIDTH)
        glVertex((self.WIDTH / 2) * Block.WIDTH, 0, self.HEIGHT/2 * Block.WIDTH)
        glVertex((self.WIDTH / 2) * Block.WIDTH, 0, -self.HEIGHT/2 * Block.WIDTH)

        glColor(255, 255, 0)
        glVertex((self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, self.HEIGHT/2 * Block.WIDTH)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, self.HEIGHT/2 * Block.WIDTH)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, 0, self.HEIGHT/2 * Block.WIDTH)
        glVertex((self.WIDTH / 2) * Block.WIDTH, 0, self.HEIGHT/2 * Block.WIDTH)

        glColor(0, 0, 255)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, -self.HEIGHT/2 * Block.WIDTH)
        glVertex((self.WIDTH / 2) * Block.WIDTH, self.ALTITUDE * Block.WIDTH, -self.HEIGHT/2 * Block.WIDTH)
        glVertex((self.WIDTH / 2) * Block.WIDTH, 0, -self.HEIGHT/2 * Block.WIDTH)
        glVertex(-(self.WIDTH / 2) * Block.WIDTH, 0, -self.HEIGHT/2 * Block.WIDTH)

        glEnd()

        glColor3f(1.0, 1.0, 1.0)
        glBegin(GL_LINES)

        for i in range(0, self.WIDTH + 1):
            glVertex3f((-self.WIDTH / 2 + i) * Block.WIDTH, 0, -(self.HEIGHT / 2) * Block.WIDTH)
            glVertex3f((-self.WIDTH / 2 + i) * Block.WIDTH, 0, (self.HEIGHT / 2) * Block.WIDTH)

            # glVertex3f((-self.WIDTH / 2 + i) * Block.WIDTH, 0, -(self.HEIGHT / 2) * Block.WIDTH)
            # glVertex3f((-self.WIDTH / 2 + i) * Block.WIDTH, Block.WIDTH * self.ALTITUDE, -(self.HEIGHT / 2) * Block.WIDTH)

            # glVertex3f((-self.WIDTH / 2 + i) * Block.WIDTH, 0, (self.HEIGHT / 2) * Block.WIDTH)
            # glVertex3f((-self.WIDTH / 2 + i) * Block.WIDTH, Block.WIDTH * self.ALTITUDE, (self.HEIGHT / 2) * Block.WIDTH)


        for i in range(0, self.HEIGHT + 1):
            glVertex3f(-(self.WIDTH / 2) * Block.WIDTH, 0, (-self.HEIGHT / 2 + i) * Block.WIDTH)
            glVertex3f((self.WIDTH / 2) * Block.WIDTH, 0, (-self.HEIGHT / 2 + i) * Block.WIDTH)

            # glVertex3f(-(self.WIDTH / 2) * Block.WIDTH, 0, (-self.HEIGHT / 2 + i) * Block.WIDTH)
            # glVertex3f(-(self.WIDTH / 2) * Block.WIDTH, Block.WIDTH * self.ALTITUDE, (-self.HEIGHT / 2 + i) * Block.WIDTH)

            # glVertex3f((self.WIDTH / 2) * Block.WIDTH, 0, (-self.HEIGHT / 2 + i) * Block.WIDTH)
            # glVertex3f((self.WIDTH / 2) * Block.WIDTH, Block.WIDTH * self.ALTITUDE, (-self.HEIGHT / 2 + i) * Block.WIDTH)

        glEnd()

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        glDisable(GL_CULL_FACE)

        for i in range(self.WIDTH):
            for j in range(self.ALTITUDE):
                for k in range(self.HEIGHT):
                    blockCenterCoords = (Block.WIDTH * (-self.WIDTH/2 + 0.5 + i),
                                         Block.WIDTH * (j + 0.5),
                                         Block.WIDTH * (-self.HEIGHT/2 + 0.5 + k))
                    Block.display(blockCenterCoords, self.view[i][j][k])

        glLineWidth(1)


    def rotate(self, angle):
        self._camera.rotate(angle)


    def appendTetromino(self, tetromino : Tetromino):
        for i in range(tetromino.REPR_WIDTH):
            for j in range(tetromino.REPR_WIDTH):
                for k in range(tetromino.REPR_WIDTH):
                    if tetromino.repr[i][j][k]:
                        # print(tetromino.xOffset + i, tetromino.yOffset + j - tetromino.REPR_WIDTH, tetromino.zOffset + k)
                        self.view[tetromino.xOffset + i][tetromino.yOffset + j - tetromino.REPR_WIDTH][tetromino.zOffset + k] = tetromino.color


    def coordsAreValid(self, x, y, z):
        return x >= 0 and x < self.WIDTH and y >= 0 and y < self.ALTITUDE and z >= 0 and z < self.HEIGHT


    def update(self):
        for i in range(self.ALTITUDE - 1, -1, -1):
            isFilled = True
            for j in range(self.WIDTH):
                for k in range(self.HEIGHT):
                    if self.view[j][i][k] == Color.TRANSPARENT:
                        isFilled = False
                        break
                else:
                    break

            if isFilled:
                for j in range(i, self.ALTITUDE - 2):
                    for k in range(self.WIDTH):
                        for t in range(self.HEIGHT):
                            self.view[k][j][t] = self.view[k][j + 1][t]

                for k in range(self.WIDTH):
                    for t in range(self.HEIGHT):
                        self.view[k][self.ALTITUDE - 1][t] = Color.TRANSPARENT