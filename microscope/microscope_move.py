# sending serial commend to Arduino

import serial


class Move:
    def __init__(self):
        self.arduinoData = serial.Serial('COM5', 9600)   # open serial communication to arduino, on COM5 with Baud-rate 9600

    def stop(self):
        self.arduinoData.write('0'.encode('utf-8'))  # stop motor

    def minus_y(self):
        self.arduinoData.write('1'.encode('utf-8'))  # sends the arduino 'l', to move to the left at max speed

    def plus_y(self):
        self.arduinoData.write('2'.encode('utf-8'))  # sends the arduino 'r'

    def plus_x(self):
        self.arduinoData.write('3'.encode('utf-8'))  # sends the arduino 'f'

    def minus_x(self):
        self.arduinoData.write('4'.encode('utf-8'))  # sends the arduino 'b'
