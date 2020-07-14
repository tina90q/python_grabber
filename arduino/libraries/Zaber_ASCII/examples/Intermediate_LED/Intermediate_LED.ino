/*
    Intermediate_LED.ino

    Intermediate example for controlling Zaber devices in the ASCII protocol.

    Queries for the Zaber device's position on each loop, and disables the Zaber device's
    LED power/status lights if the device is in the home position. Otherwise, the lights
    are enabled.
*/

#include <ZaberAscii.h>
 
ZaberShield shield(ZABERSHIELD_ADDRESS_AA);
ZaberAscii za(shield);
 
void setup() {
    /* Initialize baudrate to 115200, typical for Zaber ASCII devices */
    shield.begin(115200);
}
 
void loop() {
    /* Issue a get position command to the device, storing the response */
    za.send(1, "get pos");

    /* Create a variable to store the device response inside */ 
    ZaberAscii::reply posResponse = za.receive();

    if (posResponse.responseData == 0)
    {
        /* Set the device's LED lights to OFF if it is at home */
        za.send(1, "set system.led.enable", 0);
    }
    else 
    {
        /* Set the device's LED lights to ON */
        za.send(1, "set system.led.enable", 1);
    }

    /* Always read the reply even if you don't check it for errors. */
    za.receive();
}
