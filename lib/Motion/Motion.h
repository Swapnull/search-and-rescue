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
    bool checkForWalls(int timeout = 0);
    bool checkForRoom(int inDirection);
    void exploreRoom(int inDirection);
    void exitRoom(int inDirection);
    bool checkForEnd();
    int getDistance();
    void levelToWall();
    bool againstWall();
    bool aboveLine(int sensor);

    ZumoReflectanceSensorArray reflectanceSensors;
    unsigned int sensors[6];
    int roomCount;
private:

};
#endif
