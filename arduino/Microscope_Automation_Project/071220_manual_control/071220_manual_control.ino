//DOCUMENTATION//
//serialData::::
//'1' = move left
//'2' = move right


#include <AccelStepper.h>

// stepping sequence = Pin(1,3,2,4)
AccelStepper stepperX(AccelStepper::HALF4WIRE, 10, 12, 11, 13);
AccelStepper stepperY(AccelStepper::HALF4WIRE, 6, 8, 7, 9);


void setup() {
  Serial.begin(9600);

  
  // stepperX setup:
  stepperX.setMaxSpeed(1000);
  stepperY.setMaxSpeed(1000);
  
 
  

 
}


void loop() {
  if(Serial.available()>0){
    char serialData = Serial.read();
    long moveSpeed = 900;  // speed for manual moves at steps/s
    
    if(serialData == '1'){ //manual move left
      do{
      stepperX.setSpeed(-moveSpeed);
      stepperX.runSpeed();
        if(Serial.available()>0){      
        serialData = Serial.read();
        }
      }while(serialData != '0');
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
