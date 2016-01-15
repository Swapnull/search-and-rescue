/*
This file was written by Martyn Rushton (Swapnull) although it was based upon work done by some guys at Texas Instruments quite heavily ()
*/

#include "Motion.h"
#include "ZumoMotors.h"
#include "TurnSensor.h"

#define SENSOR_THRESHOLD 30

L3G gyro;
ZumoMotors motors;

Motion::Motion()
{
    unsigned int sensors[6];
    ZumoReflectanceSensorArray reflectanceSensors;
}

void Motion::setupReflectanceSensors(){
    reflectanceSensors.init();

    int i;
    for(int i = 0; i < 80; i++){
        if ((i > 10 && i <= 30) || (i > 50 && i <= 70)){
            motors.setSpeeds(-200, 200);
        } else {
            motors.setSpeeds(200, -200);
        }
        reflectanceSensors.calibrate();

        delay(20);
    }

    //stop motors after calibration
    motors.setSpeeds(0, 0);

}

void Motion::turn(int target){
    turnSensorReset();
    int32_t currHeading = Utilities::wrapAngle(turnSensorUpdate());
    int32_t targetAngle = Utilities::wrapAngle(currHeading + target);
    float error, power;
    bool done = false;
    while(!done){

        error = Utilities::wrapAngle(turnSensorUpdate() - targetAngle);
        done = Utilities::inRange(fabs(error), (float) -1,(float) 1);
        power = 150;
        /* Note: this is used because powers higher than 250 result
         * in a buildup of error in the integrated gyro angle
         */
        if (power > 200) {
            power = 200;
        }
        if (error > 0) {
            motors.setSpeeds(power, -power);
        }
        else {
            motors.setSpeeds(-power, power);
        }
        currHeading = turnSensorUpdate();
    }
    motors.setSpeeds(0, 0);
}

void Motion::advance(){
    //look right, see if there is a wall.
    turn(-90);
    checkForWalls();
    //look left, see if there is a wall.
    turn(90);turn(90); // 2x 90 degrees is more accurate than 1x180 for some reason
    checkForWalls();
    //advance
    turn(-90);
    motors.setSpeeds(100, 100);
}

void Motion::checkForWalls(){
    //edge fowards checking for walls.
    reflectanceSensors.readLine(sensors);
    while(!againstWall()){
        reflectanceSensors.readLine(sensors);
        if(aboveLine(sensors[0]) && aboveLine(sensors[1])){
            motors.setLeftSpeed(0);
        }else{
            motors.setLeftSpeed(100);
        }

        if(aboveLine(sensors[4]) && aboveLine(sensors[5])){
            motors.setRightSpeed(0);
        }else{
            motors.setRightSpeed(100);
        }
    }
}

bool Motion::againstWall(){

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

bool Motion::aboveLine(int sensor){
    return sensor > SENSOR_THRESHOLD;
}
