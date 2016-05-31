//
// Created by jasf on 5/12/16.
//

#include "HardDiskSimulator.h"
#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

HardDiskSimulator::HardDiskSimulator(track_array *cylinders, unsigned int qtdCylinders) :
        currentHeadPos(0, 0, 0) {

    if (cylinders == nullptr) {
        cout << "nullptr ptr received!" <<
        endl;
    }

    this->cylinders = cylinders;
    this->qtdCylinders = qtdCylinders;

}

void HardDiskSimulator::zeroOutCylinders() {
    //not necessary


}

void HardDiskSimulator::writeToSector(char *buffer, unsigned int size, unsigned int sectorIdx, double *time) {
    if (size > BYTES_PER_SECTOR) {
        throw runtime_error(
                "Are you MAD??? Size: " + to_string(size) + " Size of Sector:" + to_string(BYTES_PER_SECTOR));
    }
    SectorAddr addr = SectorAddr::getClusterDetailedAddr(sectorIdx);

    void *dst = (void *) cylinders[addr.cylinder].track[addr.trackInsideCylinder].sector[addr.sectorInsideTrack].bytes_s;

    std::memcpy(dst, (void *) buffer, size);
    cylinders[addr.cylinder].track[addr.trackInsideCylinder].sector[addr.sectorInsideTrack].qtdOfUsedBytes = (unsigned short) size;

    // updates execution time on time ptr

    //time to locate track (in case we need SEEK)
    *time += abs((int)currentHeadPos.cylinder - (int)addr.cylinder) * MIN_SEEK_TIME;

    //time to wait correct sector to be under writing head
    *time += MEAN_LATENCY_TIME;

    //time to transfer data
    *time += TRACK_TRANSFER_TIME / SECTOR_PER_TRACK;

    currentHeadPos = addr;


}

void HardDiskSimulator::readSector(unsigned int sectorIdx, char *output_buffer, unsigned short *outputSize, double *time) {
    SectorAddr addr = SectorAddr::getClusterDetailedAddr(sectorIdx);
    void *src = (void *) cylinders[addr.cylinder].track[addr.trackInsideCylinder].sector[addr.sectorInsideTrack].bytes_s;

    *outputSize = cylinders[addr.cylinder].track[addr.trackInsideCylinder].sector[addr.sectorInsideTrack].qtdOfUsedBytes;

    std::memcpy(output_buffer, src, *outputSize);

    // updates execution time on time ptr
    //time to locate track (in case we need SEEK)
    *time += abs((int)currentHeadPos.cylinder - (int)addr.cylinder) * MIN_SEEK_TIME;

    //time to wait correct sector to be under writing head
    *time += MEAN_LATENCY_TIME;

    //time to transfer data
    *time += TRACK_TRANSFER_TIME / SECTOR_PER_TRACK;

    currentHeadPos = addr;

}

unsigned short HardDiskSimulator::getQtdWrittenOnSector(unsigned int sectorIdx) {
    SectorAddr addr = SectorAddr::getClusterDetailedAddr(sectorIdx);
    return cylinders[addr.cylinder].track[addr.trackInsideCylinder].sector[addr.sectorInsideTrack].qtdOfUsedBytes;
}

void HardDiskSimulator::clearSector(unsigned int sectorIdx) {
    SectorAddr addr = SectorAddr::getClusterDetailedAddr(sectorIdx);
    cylinders[addr.cylinder].track[addr.trackInsideCylinder].sector[addr.sectorInsideTrack].qtdOfUsedBytes = 0;

}

void HardDiskSimulator::positionHead(SectorAddr pos) {
    currentHeadPos = pos;

}
