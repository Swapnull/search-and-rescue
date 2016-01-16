/*
This file was written by Martyn Rushton (Swapnull) although it was based upon work done by some guys at Texas Instruments quite heavily ()
*/

#include "Motion.h"
#include "ZumoMotors.h"
#include "TurnSensor.h"
#include "NewPing.h"
#include <SoftwareSerial.h>

#define SENSOR_THRESHOLD 30
#define TRIGGER_PIN  6
#define ECHO_PIN     2
#define MAX_DISTANCE 50

L3G gyro;
ZumoMotors motors;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
SoftwareSerial XBee(0, 1); //RX, RT

//constructor
Motion::Motion(){
    XBee.begin(9600);
    inRoom = false;
    roomCount = 0;
}

//Initialization of zumo's reflectance sensors
void Motion::setupReflectanceSensors(){
    reflectanceSensors.init();

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

//start zumo going.
bool Motion::run(){
    if(!inRoom){
        return advance();
    } else {
        checkRoom();
        inRoom = true;
        return true;
    }
}

//turn a set number of degrees
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

//look left for wall, look right for wall, move forward looking for wall.
bool Motion::advance(){
    //look right, see if there is a wall.
    turn(-90);
    if(!checkForWalls(2000)){
        checkForRoom(1); //pass 1 for right
        return false;
    }

    //look left, see if there is a wall.
    turn(90);turn(90); // 2x 90 degrees is more accurate than 1x180 due to gyro inaccuracies.
    if(!checkForWalls(2000)){
        checkForRoom(2); //pass 2 for left
        return false;
    }

    //turn back straight and advance
    turn(-90);
    motors.setSpeeds(100, 100);
    if(!checkForWalls(2000)){
        return false;
    }
}

//moves forwards looking for a wall. Returns false if not found before a timeout.
bool Motion::checkForWalls(int timeout){
    //edge fowards checking for walls.
    reflectanceSensors.readLine(sensors);
    int t = millis(); //time coming into the function.
    while(!againstWall() && ((millis() - t) < timeout)){
        //read date from sensors
        reflectanceSensors.readLine(sensors);

        //check if right side is above line
        if(aboveLine(sensors[0]) && aboveLine(sensors[1])){
            motors.setLeftSpeed(0);
        }else{
            motors.setLeftSpeed(100);
        }
        //check if left size is above line.
        if(aboveLine(sensors[4]) && aboveLine(sensors[5])){
            motors.setRightSpeed(0);
        }else{
            motors.setRightSpeed(100);
        }
    }



    if(againstWall()){
        //wall found
        //back up from found wall slightly.
        motors.setSpeeds(-100, -100);
        delay(500);
        motors.setSpeeds(0, 0);
        return true;
    }else{
        //no wall found, hit timeout
        return false;
    }
}

//checks to see if we have entered a room
bool Motion::checkForRoom(int inDirection){
    //look right, see if there is a wall.
    turn(-90);
    if(!checkForWalls(1000)){
        return false;
    }

    //look left, see if there is a wall.
    turn(90);turn(90); // 2x 90 degrees is more accurate than 1x180 due to gyro inaccuracies.
    if(!checkForWalls(2000)){
        return false;
    }

    //there is no wall to the left or right, must be in room.
    exploreRoom(inDirection);
}

//explores the room and lets the user know if we have found anything.
void Motion::exploreRoom(int inDirection){
    roomCount++;
    if(checkRoom()){
        XBee.print("An object was found in room ");
        XBee.println(roomCount);
    }else{
        XBee.print("No object was found in room ");
        XBee.println(roomCount);
    }

    turn(90);turn(90); //turn 180 to face door

    checkForWalls(0); //no timeout as we know there is a wall

    if(inDirection == 1){
        //entered from looking right. Need to turn right heading out.
        turn(-90);
    }else{
        //entered from looking left, need to turn left heading out.
        turn(90);
    }
}

//returns weather there is an object in the room
bool Motion::checkRoom(){
        int degrees = 360;
        bool found = false;
        while(degrees > 0){
            if(getDistance() > 0){
                Serial.println("Found Object");
                found = true;
            }
            turn(90);
            degrees = degrees - 90;
        }
        return found;
}

//returns distance found by ultrasonic sensor.
int Motion::getDistance(){
    return sonar.ping()/ US_ROUNDTRIP_CM;
}

//returns if zumo is against a wall or not.
bool Motion::againstWall(){

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

    if(aboveLine(sensors[0]) == 1 && aboveLine(sensors[1]) == 1 && aboveLine(sensors[4]) == 1 && aboveLine(sensors[5]) == 1){
        return true;
    } else {
        return false;
    }
}

//returns if sensor is above the wall
bool Motion::aboveLine(int sensor){
    return sensor > SENSOR_THRESHOLD;
}
