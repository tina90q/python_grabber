/*
    ZaberAscii.h - ASCII Library for interacting with Zaber devices
    January 9, 2018.
*/

#ifndef ZaberAscii_h
#define ZaberAscii_h

#include <Arduino.h>
#include "ZaberShield.h"

class ZaberAscii
{
    public:
    
        /*
            @brief Initializes the a ZaberAscii class instance on the specified serial port.
            
            @param serial 	The serial class to use (Eg: Serial, Serial1, s...)
        */
        ZaberAscii(Stream& serial);
    
        /* reply struct format returned when a command is issued */
        struct reply
        {
            /* The full reply in string format */
            String fullResponse;
            /* Evaluates to true if the reply is a reply */
            bool isReply;
            /* Evaluates to the device number the reply is in relation to */
            int deviceNumber;
            /* Evaluates to the axis number that the reply is in relation to */
            int axisNumber;
            /* Evaluates to true if the issuing of the command was rejected */
            bool isRejected;
            /* Evaluates to true if the device was busy during issuing of the command*/
            bool isBusy;
            /* Evaluates to true if the issuing of the command resulted in a warning */
            bool hasWarning;
            /* Contains the warning flag raised if hasWarning is evaluated to true */
            String warningFlag;
            /* The attached reply data in String format */
            String responseDataString;
            /* The attached reply data in long int format */
            long int responseData;
        };
        
        /*
            @brief Issues a command to the connected device with the given data.
            
            Sends a command to the device and returns with a reply struct containing the device's reply to
            the command. Uses the default axis declaration (IE, none specified, and command is sent to all axis).
            Available commands types can be found in the class Command.
            
            @param device	The device to perform the request upon
            @param command	The command to issue to the device
            @param data 	Any additional data to be sent along with the command
        */
        void send(int device, String command, long int data);
        
        /* 
            @brief Issues a command to the specified axis on the connected device with the given data.
            
            @param device	The device to perform the request upon
            @param axis 	The device axis to perform the request upon
            @param command	The command to issue to the device
            @param data 	Any additional data to be sent along with the command
        */
        void send(int device, int axis, String command, long int data);

        /*
            @brief Issues a command to the connected device without any data attached to the command.
            
            Sends a command to the device and returns with a reply struct containing the device's reply to
            the command. Uses the default axis declaration (IE, none specified, and command is sent to all axis).
            Available command types can be found in the class Command.
            
            @param device	The device to perform the request upon
            @param command	The command to issue to the device
        */
        void send(int device, String command);
        
        /*
            @brief Issues a command to the specified axis on the connected device without any data attached to the
            command.
            
            Sends a command to the specified axis on the connected device and returns with a reply struct containing
            the device's reply to the command. Uses the default axis declaration (IE, none specified, and command is
            sent to all axis). Example command types can be found in the class Command.
            
            @param device	The device to perform the request upon
            @param axis 	The device axis to perform the request upon
            @param command	The command to issue to the device
        */
        void send(int device, int axis, String command);
        
        /*
            @brief Issues a command string to the connected device.
            
            Sends command given as a String. For example, use IssueAbsoluteCommand("/1 get pos"); to issue "/1 get pos"
            to the device and receive a reply detailing device number 1's position. Returns a reply struct. Uses 
            the default axis declaration (IE, none specified, and command is sent to all axis). See:
                https://www.zaber.com/wiki/Manuals/ASCII_Protocol_Manual
            for additional ASCII protocol commands.
            
            @param string	The command string to be sent
        */
        void send(String command);

        /*
            @brief Quickly check whether a device is currently idle or moving.

            @param device 	The device number upon which to check busy status.
        */
        bool isIdle(int device);

        /*
            @brief Wait until the device has completed movement before executing the next command.
            
            @param device 	The device number upon which to poll until idle
        */
        void pollUntilIdle(int device);


        /*
            @brief Reads a message from the serial port.
        */
        reply receive();

    private:

        /* Reference to the serial class in use */
        Stream& _serialPort;

        /* Decodes a string reply from a device into a reply struct */
        reply decodeReply(String deviceReply);
};

#endif