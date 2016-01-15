#ifndef Motion_H
#define Motion_H

#include "Utilities.h"
#include "ZumoReflectanceSensorArray.h"

class Motion{
public:
    Motion();
    void setupReflectanceSensors();
    void turn(int target);
    void advance();
    void checkForWalls();
    bool againstWall();
    bool aboveLine(int sensor);

    ZumoReflectanceSensorArray reflectanceSensors;
    unsigned int sensors[6];

private:

};
#endif
