/*
This file was written by Martyn Rushton (Swapnull) although it was based upon work done by some guys at Texas Instruments quite heavily ()
*/

#include "Motion.h"
#include "ZumoMotors.h"
#include "TurnSensor.h"

L3G gyro;
ZumoMotors motors;

Motion::Motion()
{}

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
    //look left, see if there is a wall.
    turn(90);turn(90); // 2x 90 degrees is more accurate than 1x180 for some reason
    //advance
    turn(-90);
    motors.setSpeeds(100, 100);
}
