# sending serial commend to Arduino

import serial


class Move:
    def __init__(self):
        self.arduinoData = serial.Serial('COM5', 9600)   # open serial communication to arduino, on COM5 with Baud-rate 9600
        print("opening serial communication")

    def move_left(self):
        self.arduinoData.write('l'.encode('utf-8'))  # sends the arduino 'l', to move to the left at max speed
        print("output is l")

    def move_right(self):
        self.arduinoData.write('r'.encode('utf-8'))  # sends the arduino 'r'
        print("output is r")

    def move_forward(self):
        self.arduinoData.write('f'.encode('utf-8'))  # sends the arduino 'f'
        print("output is f")

    def move_backward(self):
        self.arduinoData.write('b'.encode('utf-8'))  # sends the arduino 'b'
        print("output is b")
