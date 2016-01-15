#include <Arduino.h>
#include "QTRSensors.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include "Motion.h"
#include "TurnSensor.h"
void setup();
void loop();
#line 1 "src/sketch.ino"
//#include "QTRSensors.h"
//#include <SoftwareSerial.h>
//#include <Wire.h>
//#include "Motion.h"
//#include "TurnSensor.h"



SoftwareSerial XBee(0, 1); //RX, RT
const int MAX_SPEED = 400;
Motion motion;


void setup(){
    //Initialization
    Serial.begin(9600);
    XBee.begin(9600);
    Wire.begin();
    pinMode(13, OUTPUT);

    digitalWrite(13, HIGH);
    turnSensorSetup();

    motion.setupReflectanceSensors();

    digitalWrite(13, LOW);

}

void loop(){

    motion.advance();

    Serial.println("turn command executed");

    delay(1000);

}
