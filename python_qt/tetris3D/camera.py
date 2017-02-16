from OpenGL.GL import *

class Camera:

    def rotate(self, angle):
        glMatrixMode(GL_PROJECTION)
        glRotate(angle, 0, 1, 0)
        glMatrixMode(GL_MODELVIEW)