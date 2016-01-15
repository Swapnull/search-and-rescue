#ifndef TurnAngleCommand_H
#define TurnAngleCommand_H

#include "PIDController.h"
#include "Utilities.h"

class TurnAngleCommand{
public:
    TurnAngleCommand();
    void turn(int target);
    PIDController spinController;

private:
};
#endif
