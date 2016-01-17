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
    bool againstWall();
    bool aboveLine(int sensor);
    bool checkRoom();
    int getDistance();
    bool checkForRoom(int inDirection);
    void exploreRoom(int inDirection);
    bool checkForEnd();
    void levelToWall();

    ZumoReflectanceSensorArray reflectanceSensors;
    unsigned int sensors[6];
    bool inRoom;
    int roomCount;
private:

};
#endif
