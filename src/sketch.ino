#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <SoftwareSerial.h>
#include <TurnSensor.h>
#include <L3G.h>
#include <Wire.h>


#define SENSOR_THRESHOLD 30

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
SoftwareSerial XBee(0, 1); //RX, RT
L3G gyro;
const int MAX_SPEED = 400;
unsigned int sensors[6];


void setup(){
    //Initialization
    Serial.begin(9600);
    XBee.begin(9600);
    Wire.begin();
    pinMode(13, OUTPUT);
    //Serial.println("Starting Calibration");
    //setupReflectanceSensors();
    //setupGyro();
    //Serial.println("Calibration finished");

    turnSensorSetup();
    //Serial.println(getGyroReading());
    digitalWrite(13, LOW);

}

void loop(){

/*    reflectanceSensors.readLine(sensors);

    boolean againstWall = againstEdge();
    //advanceUpCorridor();
    Serial.println(againstWall);*/
    turnSensorReset();

    motors.setSpeeds(200, -200);
    while((int32_t)turnAngle < turnAngle45 * 2){
        //Serial.println(turnAngle);
        turnSensorUpdate();
    }
    motors.setSpeeds(0, 0);


    //motors.setSpeeds(100, -100);
    //turnSensorUpdate();
    //Serial.println(getGyroReading());

    delay(3000);

}

void setupReflectanceSensors(){
    XBee.write("..Starting Relectance Sensor Calibration");
    Serial.println("..Starting Relectance Sensor Calibration");

    reflectanceSensors.init();
    //Turn on led to show calibration is happening
    digitalWrite(13, HIGH);
    //calibrate reflectance array
    delay(1000);
    int i;
    for(int i = 0; i < 80; i++){
        if ((i > 10 && i <= 30) || (i > 50 && i <= 70)){
            motors.setSpeeds(-400, 400);
        } else {
            motors.setSpeeds(400, -400);
        }
        reflectanceSensors.calibrate();

        delay(20);
    }

    //stop motors and turn off led after calibration
    motors.setSpeeds(0, 0);
    XBee.write("..Reflectance Sensors Calibrated");
    Serial.println("..Reflectance Sensors Calibrated");
}

void advanceUpCorridor(){
        if(againstWall()){
            Serial.println("Against Wall");
        }
}

void turnLeft(){
    turnSensorReset();

    motors.setSpeeds(100, -100);
    while((int32_t)turnAngle < turnAngle90){
        //Serial.println(turnAngle);
        turnSensorUpdate();
    }
    motors.setSpeeds(0, 0);
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
    /*Serial.print(aboveLine(sensors[0]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[1]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[2]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[3]));
    Serial.print(" : ");
    Serial.print(aboveLine(sensors[4]));
    Serial.print(" : ");
    Serial.println(aboveLine(sensors[5]));*/

    if(aboveLine(sensors[0]) == 1 || aboveLine(sensors[1]) == 1 || aboveLine(sensors[2]) == 1 || aboveLine(sensors[3]) == 1 || aboveLine(sensors[4]) == 1 || aboveLine(sensors[5]) == 1){
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
