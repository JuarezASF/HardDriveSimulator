//
// Created by jasf on 5/12/16.
//

#ifndef DISKSIMULATOR_HARDDISKSIMULATOR_H
#define DISKSIMULATOR_HARDDISKSIMULATOR_H

#include "Structs.h"


class HardDiskSimulator {

private:

    JTrack *cylinders;
    unsigned int qtdCylinders;

    void zeroOutCylinders();


public:
    HardDiskSimulator(track_array *cylinders);

};


#endif //DISKSIMULATOR_HARDDISKSIMULATOR_H
