//DOCUMENTATION//
//serialData::::
//'1' = move left
//'2' = move right


#include <AccelStepper.h>

// stepping sequence = Pin(1,3,2,4)
AccelStepper stepperX(AccelStepper::HALF4WIRE, 10, 12, 11, 13);
AccelStepper stepperY(AccelStepper::HALF4WIRE, 6, 8, 7, 9);

long TravelX;  // Used to store the X value entered in the Serial Monitor
int move_finished=1;  // Used to check if move is completed
long initial_homing=-1;  // Used to Home Stepper at startup

//assign assign pin that is wired to the Y-endstop button
int xEndstop = 19;
boolean xEndstopState;



void setup() {
  Serial.begin(9600);
  
  homeFunction();
  stepperY.setMaxSpeed(1000);
  
}


void loop() {
  //manual move at constant speed
  GUIManualMove(500);
}



  
void homeFunction(){
  pinMode(xEndstop, INPUT_PULLUP);
  delay(5);
  
  stepperX.setMaxSpeed(200.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(100.0);  // Set Acceleration of Stepper
  

  

  Serial.print("Stepper is Homing . . . . . . . . . . . ");

  while (digitalRead(xEndstop)) {  // Make the Stepper move CCW until the switch is activated   
    stepperX.moveTo(initial_homing);  // Set the position to move to
    initial_homing--;  // Decrease by 1 for next move if needed
    stepperX.run();  // Start moving the stepper
    delay(5); //why??
  }
  stepperX.setCurrentPosition(0);  // Set the current position as zero for now
  stepperX.setMaxSpeed(100.0);      // Set Max Speed of Stepper (Slower to get better accuracy)
  stepperX.setAcceleration(100.0);  // Set Acceleration of Stepper
  initial_homing=1;

  while (!digitalRead(xEndstop)) { // Make the Stepper move CW until the switch is deactivated
    stepperX.moveTo(initial_homing);  
    stepperX.run();
    initial_homing++;
    delay(5);
  }
  
  stepperX.setCurrentPosition(0);
  Serial.println("Homing Completed");
  Serial.println("");
  stepperX.disableOutputs();
  
  // stepperX setup:
  stepperX.setMaxSpeed(1000);
}

void GUIManualMove(long moveSpeed){
  if(Serial.available()>0){
    char serialData = Serial.read();
    
    if(serialData == '1'){ //manual move -Y
      do{
      stepperX.setSpeed(-moveSpeed);
      stepperX.runSpeed();    
        if(Serial.available()>0){      
        serialData = Serial.read();
        Serial.println(serialData);
        }
      }while(serialData != '0'); //|| yEndstopState == 1);
      stepperX.disableOutputs();
    }
 else if(serialData == '2'){ //manual move right at cons. speed
      do{
      stepperX.setSpeed(moveSpeed);
      stepperX.runSpeed();
        if(Serial.available()>0){      
        serialData = Serial.read();
        }
      }while(serialData != '0');
      stepperX.disableOutputs();
    }

    else if(serialData == '3'){ //manual move backward at cons. speed
      do{
      stepperY.setSpeed(-moveSpeed);
      stepperY.runSpeed();
        if(Serial.available()>0){      
        serialData = Serial.read();
        }
      }while(serialData != '0');
      stepperY.disableOutputs();
    }

    else if(serialData == '4'){ //manual move forward at cons. speed
      do{
      stepperY.setSpeed(moveSpeed);
      stepperY.runSpeed();
        if(Serial.available()>0){      
        serialData = Serial.read();
        }
      }while(serialData != '0');
      stepperY.disableOutputs();
    }
  }
}
