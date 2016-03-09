import serial
import time
import numpy as np
import matplotlib.pyplot as plt
from drawnow import *
import matplotlib.animation as animation

ser = serial.Serial(
    port='COM4',
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
        timeout=10)

print("connected to: " + ser.portstr)

y=np.zeros(100)
n=range(1,100)
i = 0
start = time.time()
fig, ax = plt.subplots()

line, = ax.plot(y,'-')
fig.show()

def read_adc(serial):
    x=''
    for c in serial.readline():
        x=x+chr(c)
    return int(x)

def init():
    line.set_ydata(np.ma.array(0, mask=True))
    return line,

def animate(i):
    y[i]=float(int(read_adc(ser))*5/1024)
    #y=np.delete(y,99)
    #y=np.append(np.array([int(read_adc(ser))]),y)
    ax.clear()
    ax.plot(y,'-')


ani = animation.FuncAnimation(fig, animate,interval=100,blit=False)
plt.show()
ser.close()
