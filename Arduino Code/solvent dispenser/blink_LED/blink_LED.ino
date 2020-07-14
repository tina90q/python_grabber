/*

  The circuit:
 start button:
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

Blue LED = water
  -water attached to pin12 and connectter to base of 2n2222 BJT
  -dispensing time = 3 seconds

RED LED = Acetone. pin8


YELLO LED = Isoproylpanol


*/

//Set up pin nuumber
const int buttonPin = 2;     // the number of the pushbutton pin
const int waterPin =  12;      // the number of the waterpin
const int acetonePin =  8;      // the number of the acetone
const int IPAPin =  7;      // the number of the IPA

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
//long previousMillis = 0;        // will store last time LED was updated
 
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
//long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  // initialize the LED pin as an output:
  pinMode(waterPin, OUTPUT);
  pinMode(acetonePin, OUTPUT);
  pinMode(IPAPin, OUTPUT);
  
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  //Serial.begin(9600); // open the serial port at 9600 bps:
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  //unsigned long currentMillis = millis();

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on
    digitalWrite(waterPin, HIGH);
    delay(3000);
    digitalWrite(waterPin, LOW);
    digitalWrite(acetonePin, HIGH);
    delay(3000);
    digitalWrite(acetonePin, LOW);
    digitalWrite(IPAPin, HIGH);
    delay(3000);
    digitalWrite(IPAPin, LOW);
    //Serial.println("DONE");
  }
  else{
    //Serial.println("OFF");
    digitalWrite(waterPin, LOW);
    digitalWrite(acetonePin, LOW);
    digitalWrite(IPAPin, LOW);
  }
}
