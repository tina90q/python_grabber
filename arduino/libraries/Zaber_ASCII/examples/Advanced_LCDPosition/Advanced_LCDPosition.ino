/*
    Advanced_LED.ino

    Advanced example for controlling Zaber devices in the ASCII protocol.

    Queries for the Zaber device's position on each loop, converts the position to a 
    human-readable format, and prints the converted position to an LCD screen attatched
    to the Arduino. The LCD screen utilized in this example is a DFRobot LCD shield:

        https://www.dfrobot.com/product-51.html
*/

#include <ZaberAscii.h>
#include <LiquidCrystal.h>
 
/* 
    Initialize an instance of the LCD class, passing in the appropriate pins:
 
        LCD(rs, enable, d4, d5, d6, d7)
 
    These pin numbers can be found by checking the appropriate LCD manufacturer page for your screen    
*/
LiquidCrystal LCD(8, 9, 4, 5, 6, 7);
 
/* 
    Define microstep size for conversion into millimeter displacement
    To see the microstep size (default resolution) for the device, go to the product overview page:
 
        https://www.zaber.com/products/
 
    Find your device and click on the "Series Specs" tab. The microstep size (default resolution) will be shown in
    the list of product specs either in the "Group Specifications" section or the "Comparison" section
*/
float MM_RESOLUTION = 0.000049609375;
 
ZaberShield shield(ZABERSHIELD_ADDRESS_AA);
ZaberAscii za(shield);
 
void setup()
{
    /* Initialize the LCD screen of size 16x2 */
    LCD.begin(16, 2);
 
    /* Initialize baudrate to 115200, typical for Zaber ASCII devices */
    shield.begin(115200);
}
 
void loop()
{
    /* Set the LCD cursor to the top row */
    LCD.setCursor(0, 0);
    
    /* Issue a get position command to the device, storing the response */
    za.send(1, "get pos");

    /* Create a variable to store the device response inside */
    ZaberAscii::reply posResponse = za.receive();

    /* Convert the position response data to a readable format, and print it to the LCD screen */
    LCD.print("Position: " + mm(posResponse.responseData));
}
 
/* Translates the given microstep size into displacement in millimeters */
String mm(double dataValue)
{
    double mmValue;
    mmValue = (dataValue * MM_RESOLUTION);
    return String(mmValue);
}
