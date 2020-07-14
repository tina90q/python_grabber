#include <ZaberAscii.h>

ZaberShield shield(ZABERSHIELD_ADDRESS_AA);
ZaberAscii za(shield);

void setup() {
    shield.begin(115200);

    // This example uses the serial monitor for output and a Zaber shield 
    // for device interfacing at the same time. This conbination makes
    // debugging easier.
    Serial.begin(9600);
    Serial.println("Starting...");
}

void loop() {

  // Broadcast a command to all devices connected to the Zaber shield.
  za.send(0, "get pos");

  // Process every reply that comes back.
  // Checking for bytes available at the serial port prevents us from blocking
  // our program until there is a reply from a device.
  while (shield.available() > 0)
  {
    ZaberAscii::reply reply = za.receive();
    if (!reply.isReply)
    {
      Serial.println("*** Received a non-reply message from device " + String(reply.deviceNumber) + ".");
    }
    else if (reply.isRejected)
    {
      Serial.println("*** A command was rejected by device " + String(reply.deviceNumber) + ".");
    }
    else
    {
      Serial.println("Position of device " + String(reply.deviceNumber) + ": " + String(reply.responseData));
    }
  }

  // At this point we've processed every available received message, so
  // we could do some other work while waiting for more to arrive.
  delay(100);
}
