#include "QTRSensors.h"
#include <Wire.h>
#include "Motion.h"
#include "TurnSensor.h"
#include "Pushbutton.h"

const int MAX_SPEED = 400;
Motion motion;
Pushbutton button(ZUMO_BUTTON);




void setup(){
    //Initialization
    Serial.begin(9600);
    Wire.begin();
    pinMode(13, OUTPUT);

    digitalWrite(13, HIGH);
    turnSensorSetup();
    Serial.println("Press button to start calibration...");
    button.waitForButton();

    motion.setupReflectanceSensors();

    Serial.println("Calibrated. Press button to start...");
    button.waitForButton();

    digitalWrite(13, LOW);

}

void loop(){

    motion.run();

    delay(1000);

}
