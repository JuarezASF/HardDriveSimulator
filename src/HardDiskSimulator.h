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
    HardDiskSimulator(track_array *cylinders, unsigned int qtdCylinders);

    void writeToSector(char *buffer, unsigned int size, unsigned int sectorIdx);

    void clearSector(unsigned int sectorIdx);

    void readSector(unsigned int sectorIdx, char *output_buffer, unsigned short *outputSize);

    unsigned short getQtdWrittenOnSector(unsigned int sectorIdx);

};


#endif //DISKSIMULATOR_HARDDISKSIMULATOR_H
