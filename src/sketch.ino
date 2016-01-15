#include "QTRSensors.h"
#include "ZumoReflectanceSensorArray.h"
#include "ZumoMotors.h"
#include "ZumoBuzzer.h"
#include <Pushbutton.h>
#include <SoftwareSerial.h>
#include <TurnSensor.h>
#include <L3G.h>
#include <Wire.h>
#include "Motion.h"


#define SENSOR_THRESHOLD 30

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
//ZumoMotors motors;
SoftwareSerial XBee(0, 1); //RX, RT
const int MAX_SPEED = 400;
unsigned int sensors[6];
Motion motion;
int power = 0;

void setup(){
    //Initialization
    Serial.begin(9600);
    XBee.begin(9600);
    Wire.begin();
    pinMode(13, OUTPUT);

    digitalWrite(13, HIGH);

    turnSensorSetup();
    setupReflectanceSensors();

    digitalWrite(13, LOW);

}

void loop(){

    reflectanceSensors.readLine(sensors);

    motion.advance();
    Serial.println("turn command executed");

    delay(500);

}

void setupReflectanceSensors(){
    reflectanceSensors.init();

    int i;
    for(int i = 0; i < 80; i++){
        if ((i > 10 && i <= 30) || (i > 50 && i <= 70)){
            //motors.setSpeeds(-200, 200);
        } else {
            //motors.setSpeeds(200, -200);
        }
        reflectanceSensors.calibrate();

        delay(20);
    }

    //stop motors after calibration
    //motors.setSpeeds(0, 0);

}



/*
* function :
*   Find out if the zumo is against a wall.
* in:
*   N/A
* out:
*   boolean - True if against wall, false if not.
*/
boolean againstWall(){

    // DEBUG : Print out if the sensors think they are above a line.
    Serial.print(aboveLine(sensors[0]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[1]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[2]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[3]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[4]));
    Serial.print(" : ");
    Serial.println(aboveLine(sensors[5]));

    if(aboveLine(sensors[0]) == 1 && aboveLine(sensors[1]) == 1 && aboveLine(sensors[4]) == 1 && aboveLine(sensors[5]) == 1){
        return true;
    } else {
        return false;
    }
}

/*
* function:
*   Is the certain sensor above a line by comparing it to the sensor threshold;
* in:
*   int sensor - The reading from a specific sensor
* out:
*   boolean - true if above line, false if not.
*/
boolean aboveLine(int sensor){
    return sensor > SENSOR_THRESHOLD;
}
