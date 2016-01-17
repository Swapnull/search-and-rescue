/*
This file was written by Martyn Rushton (Swapnull) although it was based upon work done by some guys at Texas Instruments quite heavily ()
*/

#include "Motion.h"
#include "ZumoMotors.h"
#include "TurnSensor.h"
#include "NewPing.h"

#define SENSOR_THRESHOLD 30
#define TRIGGER_PIN  6
#define ECHO_PIN     2
#define MAX_DISTANCE 50
#include "Pushbutton.h"
Pushbutton b(ZUMO_BUTTON);


L3G gyro;
ZumoMotors motors;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//constructor
Motion::Motion(){
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
    return advance();
}

//turn a set number of degrees
void Motion::turn(int target){

    Serial.print("turning ");
    if(target > 0)
        Serial.println("anticlockwise.");
    else
        Serial.println("clockwise");

    turnSensorReset();
    int32_t currHeading = Utilities::wrapAngle(turnSensorUpdate());
    int32_t targetAngle = Utilities::wrapAngle(currHeading + target);
    float error, speed;
    bool done = false;
    while(!done){

        error = Utilities::wrapAngle(turnSensorUpdate() - targetAngle);
        done = Utilities::inRange(fabs(error), (float) -1,(float) 1);

        speed = 150;
        /*speed = (error * 6.5) + ((int)(error * 0.001));
        Serial.print(error);
        Serial.print(" : ");
         /* Note: this is used because powers higher than 250 result
         * in a buildup of error in the integrated gyro angle
         *
        if (speed > 200) {
            speed = 100;
        }*/

        if (error > 0) {
            motors.setSpeeds(speed, -speed);
        }
        else {
            motors.setSpeeds(-speed, speed);
        }
    }
    motors.setSpeeds(0, 0);
}

//look left for wall, look right for wall, move forward looking for wall.
bool Motion::advance(){
    Serial.println("Advancing");
    //look right, see if there is a wall.
    turn(-90);
    if(!checkForWalls(2000)){
        checkForRoom(1); //pass 1 for right
    }

    //look left, see if there is a wall.
    turn(90);turn(90); // 2x 90 degrees is more accurate than 1x180 due to gyro inaccuracies.
    if(!checkForWalls(2000)){
        checkForRoom(2); //pass 2 for left
    }

    //turn back straight and advance
    turn(-90);
    motors.setSpeeds(100, 100);
    if(checkForWalls(500)){
        checkForEnd();
        return false;
    }
}

//moves forwards looking for a wall. Returns false if not found before a timeout.
bool Motion::checkForWalls(int timeout){
    //edge fowards checking for walls.
    Serial.println("checking for wall");
    reflectanceSensors.readLine(sensors);
    int t = millis(); //time coming into the function.
    while(againstWall() != 1 && (((millis() - t) < timeout) || timeout == 0)){
        //read date from sensors
        reflectanceSensors.readLine(sensors);

        if(againstWall() == 2 || againstWall() == 3){
            break;
        }
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
        levelToWall();
        return true;
    }else{
        //no wall found, hit timeout
        return false;
    }
}

//checks to see if we have entered a room
bool Motion::checkForRoom(int inDirection){
    //look right, see if there is a wall.
    Serial.println("Checking for room");
    turn(-90);
    bool foundRight = checkForWalls(500);

    //look left, see if there is a wall.
    turn(90);turn(90); // 2x 90 degrees is more accurate than 1x180 due to gyro inaccuracies.
    bool foundLeft = checkForWalls(500);

    //there is no wall to the left or right, must be in room.
    if(!foundLeft && !foundRight ){
        exploreRoom(inDirection);
    }
    Serial.println("Exiting Room");
    //b.waitForButton();

    //Serial.println("Turning to leave");
    //turn(90);turn(90); //turn 180 to face door
    b.waitForButton();

    motors.setSpeeds(100, 100);
    checkForWalls(0); //no timeout as we know there is a wall
    b.waitForButton();

    if(inDirection == 1){
        //entered from looking right. Need to turn right heading out.
        turn(-90);
    }else{
        //entered from looking left, need to turn left heading out.
        turn(90);
    }

    Serial.println("Room should be exited");

}

//explores the room and lets the user know if we have found anything.
void Motion::exploreRoom(int inDirection){
    roomCount++;
    Serial.println("Exploring Room");
    for(int i = 0; i <= 4; i++){
        if(getDistance() > 0){
            Serial.print("An object was found in room ");
            Serial.println(roomCount);
        }
        turn(90);
        delay(500);
    //    b.waitForButton();
    }

    Serial.print("No object was found in room ");
    Serial.println(roomCount);
    Serial.println("Room explored");
    delay(100);
}

bool Motion::checkForEnd(){
        turn(-90);
        if(!checkForWalls(2000)){
            return false;
        }

        turn(90);turn(90);
        if(!checkForWalls(2000)){
            return false;
        }

        //a wall has been found ahead, left and right, must be the end.
        Serial.println("Reached end of corridor, returning to base");
        return true;
}

//returns distance found by ultrasonic sensor.
int Motion::getDistance(){
    return sonar.ping()/ US_ROUNDTRIP_CM;
}

void Motion::levelToWall(){
    //first level so both sensors are on wall
    //check if right side is above line

    while(!againstWall()){
        reflectanceSensors.readLine(sensors);
        if(aboveLine(sensors[0]) != 1 && aboveLine(sensors[1]) != 1){
            motors.setLeftSpeed(0);
        }else{
            motors.setLeftSpeed(80);
        }
        //check if left size is above line.
        if(aboveLine(sensors[4]) != 1 && aboveLine(sensors[5]) != 1){
            motors.setRightSpeed(0);
        }else{
            motors.setRightSpeed(80);
        }
    }

    //due to the wall been about 2cm thick, we should level to the corridor.
    //check if right side is above line
    while(againstWall()){
        reflectanceSensors.readLine(sensors);
        if(aboveLine(sensors[0])){
            motors.setLeftSpeed(-80);
        }else{
            motors.setLeftSpeed(0);
        }
        //check if left size is above line.
        if(aboveLine(sensors[5])){
            motors.setRightSpeed(-80);
        }else{
            motors.setRightSpeed(0);
        }
    }

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

    if(aboveLine(sensors[0]) && aboveLine(sensors[1]) && aboveLine(sensors[4]) && aboveLine(sensors[5])){
        return 1;
    } else if(aboveLine(sensors[0]) && aboveLine(sensors[1]) && aboveLine(sensors[4])) {
        return 2;
    } else if(aboveLine(sensors[1]) && aboveLine(sensors[4]) && aboveLine(sensors[5])){
        return 3;
    } else {
        return false;
    }
}

//returns if sensor is above the wall
bool Motion::aboveLine(int sensor){
    return sensor > SENSOR_THRESHOLD;
}
