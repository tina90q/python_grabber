/*
    Basic_Homing.ino

    Basic example for controlling Zaber devices in the ASCII protocol.

    Sends a Zaber device to its home position on startup.
*/

#include <ZaberAscii.h>

ZaberShield shield(ZABERSHIELD_ADDRESS_AA);
ZaberAscii za(shield);
 
void setup() {
    /* Initialize baudrate to 115200, typical for Zaber ASCII devices */
    shield.begin(115200);

    /* Issue a home command to device 1 */
    za.send(1, "home");

    /* Always read the reply even if you don't check it for errors. */
    za.receive();
}
 
void loop() {
 
}
