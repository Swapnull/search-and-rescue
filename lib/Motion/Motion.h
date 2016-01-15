#ifndef Motion_H
#define Motion_H

#include "Utilities.h"
#include "ZumoReflectanceSensorArray.h"

class Motion{
public:
    Motion();
    void setupReflectanceSensors();
    bool run();
    void turn(int target);
    bool advance();
    bool checkForWalls();
    bool againstWall();
    bool aboveLine(int sensor);
    bool checkRoom();
    int getDistance();

    ZumoReflectanceSensorArray reflectanceSensors;
    unsigned int sensors[6];
    bool inRoom;

private:

};
#endif
