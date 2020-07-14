/*
    ZaberAscii.cpp - Implements the ASCII library for interacting with Zaber devices
    January 11, 2018
    See ZaberAscii.h for documentation
*/

#include "ZaberAscii.h"

ZaberAscii::ZaberAscii(Stream& serial):
    _serialPort(serial)
{
    /*  An empty constructor is required in order to correctly initialize the serial reference before the body  */
}

/* Helper function for decoding replies */
ZaberAscii::reply ZaberAscii::decodeReply(String deviceReply)
{
    /* Read the reply */
    String messageType = deviceReply.substring(0, 1);
    String deviceAddressStr = deviceReply.substring(1, 3);
    String axisStr = deviceReply.substring(4, 5);
    String rejectedStr = deviceReply.substring(6, 8);
    String busyStr = deviceReply.substring(9, 13);
    String warningFlagStr = deviceReply.substring(14, 16);
    String dataStr = deviceReply.substring(17, deviceReply.length() - 1);

    bool isReply = messageType.equals("@");
    int deviceAddress = deviceAddressStr.toInt();
    int axisNumber = axisStr.toInt();
    bool isRejected = rejectedStr.equals("RJ");
    bool isBusy = busyStr.equals("BUSY");
    bool hasWarning = !(warningFlagStr.equals("--"));
    long int responseData = dataStr.toInt();

    /* Form and return a reply struct with the read properties*/
    reply resp = {deviceReply, isReply, deviceAddress, axisNumber, isRejected,
        isBusy, hasWarning, warningFlagStr, dataStr, responseData };
        
    return resp;
}

ZaberAscii::reply ZaberAscii::receive()
{
    String deviceReply = _serialPort.readStringUntil('\n');
    return decodeReply(deviceReply);
}


void ZaberAscii::send(int device, String command, long int data)
{
    _serialPort.println("/" + String(device) + " " + command + " " + String(data));
}

void ZaberAscii::send(int device, int axis, String command, long int data)
{
    _serialPort.println("/" + String(device) + " " + String(axis) + " " + command + " " + String(data));
}

void ZaberAscii::send(int device, String command)
{
    _serialPort.println("/" + String(device) + " " + command);
}

void ZaberAscii::send(int device, int axis, String command)
{
    _serialPort.println("/" + String(device) + " " + String(axis) + " " + command);
}

void ZaberAscii::send(String command)
{
    String cmd = command;

    /* Add a / if one is not found at the start of the string */
    if(!cmd.startsWith("/"))
    {
        cmd = ("/" + cmd);
    }

    /* Remove trailing and leading whitespace as well as any occurences of \n */
    cmd.trim();

    _serialPort.println(cmd);
}

void ZaberAscii::pollUntilIdle(int device)
{
    /* Delay in 10 millisecond increments until the device is no longer busy */
    send(device, "");
    ZaberAscii::reply resp = receive();

    while(resp.isBusy)
    {
        delay(10);
        send(device, "");
        resp = receive();
    }
}

bool ZaberAscii::isIdle(int device)
{
    send(device, "");
    ZaberAscii::reply resp = receive();
    return !resp.isBusy;
}
