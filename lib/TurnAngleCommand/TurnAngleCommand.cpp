#include "TurnAngleCommand.h"
#include "ZumoMotors.h"
//#include "IMUManager.h"
#include "TurnSensor.h"
#include "PIDController.h"

L3G gyro;
ZumoMotors motors;
PIDController spinController = PIDController((float) 6.5, (float) 0.001, (float) 0.0);

TurnAngleCommand::TurnAngleCommand()
{}

void TurnAngleCommand::turn(int target){
    turnSensorReset();
    int32_t currHeading = Utilities::wrapAngle(turnSensorUpdate());
    int32_t targetAngle = Utilities::wrapAngle(currHeading + target);
    float error, power;
    Serial.println("turning");
    Serial.println(currHeading);
    Serial.println(targetAngle);
    bool done = false;
    while(!done){
        Serial.print(currHeading);
        Serial.print(" : ");
        error = Utilities::wrapAngle(turnSensorUpdate() - targetAngle);
        Serial.print(error);
        Serial.print(" : ");
        Serial.print(power);
        Serial.print(" : ");
        done = Utilities::inRange(fabs(error), (float) -1,(float) 1);

        power = 150;
        Serial.println(power);

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
        Serial.println(done);

    }
    motors.setSpeeds(0, 0);
}
