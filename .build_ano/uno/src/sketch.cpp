#include <Arduino.h>
#include "QTRSensors.h"
#include <Wire.h>
#include "Motion.h"
#include "TurnSensor.h"
#include "Pushbutton.h"
void setup();
void loop();
#line 1 "src/sketch.ino"
//#include "QTRSensors.h"
//#include <Wire.h>
//#include "Motion.h"
//#include "TurnSensor.h"
//#include "Pushbutton.h"

const int MAX_SPEED = 400;
Motion motion;
Pushbutton button(ZUMO_BUTTON);
bool res;
int tries;



void setup(){
    //Initialization
    Serial.begin(9600);
    Wire.begin();
    pinMode(13, OUTPUT);

    digitalWrite(13, HIGH);
    turnSensorSetup();

    /*button.waitForButton();

    motion.setupReflectanceSensors();

    button.waitForButton();*/

    digitalWrite(13, LOW);

}

void loop(){

    /*if(!motion.run()){
        tries++;
        if(tries >= 3){
            tries = 0;
            motion.inRoom = true;
        }
    }*/
    //Serial.println();
    motion.checkRoom();
    delay(1000);

}
