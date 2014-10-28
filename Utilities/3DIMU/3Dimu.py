# This code is provided under the BSD license.
# Copyright (c) 2014, Emlid Limited. All rights reserved.
# Written by Mikhail Avkhimenia (mikhail.avkhimenia@emlid.com)
# twitter.com/emlidtech || www.emlid.com || info@emlid.com
#
# This application visualizes Navio's IMU data. Tested to work under Ubuntu and 
# Mac OS X. It listens for data on the UDP port 7000. To run it type:
# python 3Dimu.py

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import serial
import os
import threading
import socket
import select

# Initial rotation
x = 0.7325378163287418
y = 0.4619397662556433
z = -0.19134171618254486
w = 0.4619397662556433

#----------------------------------- Cube--------------------------------------

# vertices = [
#     -1.0, -1.0, -1.0,  -1.0, -1.0,  1.0,  -1.0,  1.0,  1.0,  -1.0,  1.0, -1.0,
#      1.0, -1.0, -1.0,   1.0, -1.0,  1.0,   1.0,  1.0,  1.0,   1.0,  1.0, -1.0,
#     -1.0, -1.0, -1.0,  -1.0, -1.0,  1.0,   1.0, -1.0,  1.0,   1.0, -1.0, -1.0,
#     -1.0,  1.0, -1.0,  -1.0,  1.0,  1.0,   1.0,  1.0,  1.0,   1.0,  1.0, -1.0,
#     -1.0, -1.0, -1.0,  -1.0,  1.0, -1.0,   1.0,  1.0, -1.0,   1.0, -1.0, -1.0,
#     -1.0, -1.0,  1.0,  -1.0,  1.0,  1.0,   1.0,  1.0,  1.0,   1.0, -1.0,  1.0]

#------------------------------ Parallelepiped --------------------------------

vertices = [
    -1.0, -2.0, -0.5,  -1.0, -2.0,  0.5,  -1.0,  2.0,  0.5,  -1.0,  2.0, -0.5,
     1.0, -2.0, -0.5,   1.0, -2.0,  0.5,   1.0,  2.0,  0.5,   1.0,  2.0, -0.5,
    -1.0, -2.0, -0.5,  -1.0, -2.0,  0.5,   1.0, -2.0,  0.5,   1.0, -2.0, -0.5,
    -1.0,  2.0, -0.5,  -1.0,  2.0,  0.5,   1.0,  2.0,  0.5,   1.0,  2.0, -0.5,
    -1.0, -2.0, -0.5,  -1.0,  2.0, -0.5,   1.0,  2.0, -0.5,   1.0, -2.0, -0.5,
    -1.0, -2.0,  0.5,  -1.0,  2.0,  0.5,   1.0,  2.0,  0.5,   1.0, -2.0,  0.5]

colors = [
    0.33, 0.75, 0.59,  0.33, 0.75, 0.59,  0.33, 0.75, 0.59,  0.33, 0.75, 0.59,
    0.33, 0.75, 0.59,  0.33, 0.75, 0.59,  0.33, 0.75, 0.59,  0.33, 0.75, 0.59,
    0.59, 0.82, 0.70,  0.59, 0.82, 0.70,  0.59, 0.82, 0.70,  0.59, 0.82, 0.70,
    0.59, 0.82, 0.70,  0.59, 0.82, 0.70,  0.59, 0.82, 0.70,  0.59, 0.82, 0.70,
    0.68, 0.90, 0.80,  0.68, 0.88, 0.80,  0.68, 0.88, 0.80,  0.68, 0.88, 0.80,
    0.68, 0.88, 0.80,  0.68, 0.88, 0.80,  0.68, 0.88, 0.80,  0.68, 0.88, 0.80]

# ================================ Main loop ==================================

def Draw():
    global x, y, z, w

    # --------------------- --- Parsing IMU data ------------------------------

    while True:
        ready = select.select([socket_in_ahrs], [], [], 0.025)
        if ready[0]:
            data = socket_in_ahrs.recv(80).split()

            if len(data) >= 4:
                w = float(data[0])
                x = float(data[1])
                y = float(data[2])
                z = float(data[3])
        else:
            break

    # ---------------- 3D transfomations and visualization --------------------

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glLoadIdentity()

    gluLookAt(0, -10, 0, 0, 0, 0, 0, 0, 1)

    qrmatrix = [1.0-2.0*(y*y+z*z),  2.0*(x*y+w*z),      2.0*(x*z-w*y),      0.0,
                2.0*(x*y-w*z),      1.0-2.0*(x*x+z*z),  2.0*(y*z+w*x),      0.0,
                2.0*(x*z+w*y),      2.0*(y*z-w*x),      1.0-2.0*(x*x+y*y),  0.0,
                0.0,                0.0,                0.0,                1.0]

    glMultMatrixf(qrmatrix);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_QUADS, 0, 24);

    glWindowPos2i(20, 20)

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers()

#================================ Main ========================================

socket_in_ahrs = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
socket_in_ahrs.bind(("0.0.0.0", 7000))

glutInit(sys.argv)
glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE)
glutInitWindowSize(960, 540)
glutCreateWindow("Emlid IMU visualizer")
glutDisplayFunc(Draw)
glutIdleFunc(Draw)

# Fullscreen mode
# glutGameModeString("1920x1080:32@50")
# glutEnterGameMode()

glClearColor(0.344, 0.347, 0.355, 1)
glClearDepth(1.0)
glDepthFunc(GL_LESS)
glEnable(GL_DEPTH_TEST)
glShadeModel(GL_SMOOTH)

glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

glMatrixMode(GL_PROJECTION)
glLoadIdentity()
gluPerspective(45.0, 960.0 / 540.0, 0.1, 100.0)
glMatrixMode(GL_MODELVIEW)

glutMainLoop()
