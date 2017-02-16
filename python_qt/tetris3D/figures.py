from OpenGL.GL import *

from enum import Enum

class Color(Enum):
    TRANSPARENT = 0 # no color
    RED = 1
    YELLOW = 2
    GREEN = 3
    BLUE = 4
    PURPLE = 5
    BLACK = 6

    def arrRepr(self, arr):
        if self == Color.TRANSPARENT:
            return
        elif self == Color.RED:
            arr[0] = 225
            arr[1] = 0
            arr[2] = 0
        elif self == Color.YELLOW:
            arr[0] = 225
            arr[1] = 225
            arr[2] = 0
        elif self == Color.GREEN:
            arr[0] = 0
            arr[1] = 225
            arr[2] = 0
        elif self == Color.BLUE:
            arr[0] = 0
            arr[1] = 0
            arr[2] = 225
        elif self == Color.PURPLE:
            arr[0] = 225
            arr[1] = 0
            arr[2] = 225
        elif self == Color.BLACK:
            arr[0] = 0
            arr[1] = 0
            arr[2] = 0

class Cube:
    _color = [0, 0, 0]
    _circuit_color = [0, 0, 0]

    @staticmethod
    def display(center, color: Color, circuit_color=Color.BLACK):

        if color == Color.TRANSPARENT:
            return
        else:
            color.arrRepr(Cube._color)

        circuit_color.arrRepr(Cube._circuit_color)


        glPushMatrix()
        glTranslate(center[0], center[1], center[2])

        glColor(Cube._color)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5, -0.5, -0.5 )
        glVertex3f(  0.5,  0.5, -0.5 )
        glVertex3f( -0.5,  0.5, -0.5 )
        glVertex3f( -0.5, -0.5, -0.5 )
        glEnd()

        glColor(Cube._circuit_color)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5, -0.5, -0.5 )
        glVertex3f(  0.5,  0.5, -0.5 )
        glVertex3f( -0.5,  0.5, -0.5 )
        glVertex3f( -0.5, -0.5, -0.5 )
        glEnd()
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

        glColor(Cube._color)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5, -0.5, 0.5 )
        glVertex3f(  0.5,  0.5, 0.5 )
        glVertex3f( -0.5,  0.5, 0.5 )
        glVertex3f( -0.5, -0.5, 0.5 )
        glEnd()

        glColor(Cube._circuit_color)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5, -0.5, 0.5 )
        glVertex3f(  0.5,  0.5, 0.5 )
        glVertex3f( -0.5,  0.5, 0.5 )
        glVertex3f( -0.5, -0.5, 0.5 )
        glEnd()
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

        glColor(Cube._color)
        glBegin(GL_POLYGON)
        glVertex3f( 0.5, -0.5, -0.5 )
        glVertex3f( 0.5,  0.5, -0.5 )
        glVertex3f( 0.5,  0.5,  0.5 )
        glVertex3f( 0.5, -0.5,  0.5 )
        glEnd()

        glColor(Cube._circuit_color)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glBegin(GL_POLYGON)
        glVertex3f( 0.5, -0.5, -0.5 )
        glVertex3f( 0.5,  0.5, -0.5 )
        glVertex3f( 0.5,  0.5,  0.5 )
        glVertex3f( 0.5, -0.5,  0.5 )
        glEnd()
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

        glColor(Cube._color)
        glBegin(GL_POLYGON)
        glVertex3f( -0.5, -0.5,  0.5 )
        glVertex3f( -0.5,  0.5,  0.5 )
        glVertex3f( -0.5,  0.5, -0.5 )
        glVertex3f( -0.5, -0.5, -0.5 )
        glEnd()

        glColor(Cube._circuit_color)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glBegin(GL_POLYGON)
        glVertex3f( -0.5, -0.5,  0.5 )
        glVertex3f( -0.5,  0.5,  0.5 )
        glVertex3f( -0.5,  0.5, -0.5 )
        glVertex3f( -0.5, -0.5, -0.5 )
        glEnd()
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

        glColor(Cube._color)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5,  0.5,  0.5 )
        glVertex3f(  0.5,  0.5, -0.5 )
        glVertex3f( -0.5,  0.5, -0.5 )
        glVertex3f( -0.5,  0.5,  0.5 )
        glEnd()

        glColor(Cube._circuit_color)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5,  0.5,  0.5 )
        glVertex3f(  0.5,  0.5, -0.5 )
        glVertex3f( -0.5,  0.5, -0.5 )
        glVertex3f( -0.5,  0.5,  0.5 )
        glEnd()
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

        glColor(Cube._color)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5, -0.5, -0.5 )
        glVertex3f(  0.5, -0.5,  0.5 )
        glVertex3f( -0.5, -0.5,  0.5 )
        glVertex3f( -0.5, -0.5, -0.5 )
        glEnd()

        glColor(Cube._circuit_color)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
        glBegin(GL_POLYGON)
        glVertex3f(  0.5, -0.5, -0.5 )
        glVertex3f(  0.5, -0.5,  0.5 )
        glVertex3f( -0.5, -0.5,  0.5 )
        glVertex3f( -0.5, -0.5, -0.5 )
        glEnd()
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

        glPopMatrix()
