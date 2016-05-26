#include <stdexcept>
#include <iostream>
#include "Structs.h"


SectorAddr::SectorAddr() {
    cylinder = 0;
    trackInsideCylinder = 0;
    sectorInsideTrack = 0;
    clusterInsideTrack = 0;
    sectorInsideCluster = 0;
    sectorInsideCylinder = 0;
    sectorInsideDisk = 0;
}

SectorAddr SectorAddr::getClusterDetailedAddr(unsigned int absolute) {
    SectorAddr out;

    if (absolute > LAST_VALID_SECTOR_ADDR){
        std::cerr << "INVALID absulute sector addr!" << std::endl;
    }

    out.sectorInsideDisk = absolute;
    out.sectorInsideCluster = absolute % SECTORS_PER_CLUSTER;
    out.sectorInsideTrack = absolute % SECTOR_PER_TRACK;
    out.sectorInsideCylinder = absolute % SECTORS_PER_CYLINDER;

    out.cylinder = absolute / SECTORS_PER_CYLINDER;

    out.trackInsideCylinder = out.sectorInsideCylinder / SECTOR_PER_TRACK;

    out.clusterInsideTrack = out.sectorInsideTrack / CLUSTERS_PER_TRACK;

    return out;
}


SectorAddr &SectorAddr::operator=(const SectorAddr &A) {
    cylinder = A.cylinder;
    trackInsideCylinder = A.trackInsideCylinder;
    sectorInsideTrack = A.sectorInsideTrack;
    clusterInsideTrack = A.clusterInsideTrack;
    sectorInsideCluster = A.sectorInsideCluster;
    sectorInsideCylinder = A.sectorInsideCylinder;
    sectorInsideDisk = A.sectorInsideDisk;

    return *this;
}


SectorAddr::SectorAddr(uint cyl, uint trackInCyl, uint clusterInTrack) {
    if(clusterInTrack >= CLUSTERS_PER_TRACK){
        throw std::runtime_error("Invalid cluster addr inside track!");
    }
    if(trackInCyl >= TRACK_PER_CYLINDER){
        throw std::runtime_error("Invalid track addr inside cylinder!");
    }
    if(cyl >= QTD_CYLINDERS){
        throw std::runtime_error("Invalid cylinder number!");
    }
   *this = SectorAddr::getClusterDetailedAddr(cyl * SECTORS_PER_CYLINDER + trackInCyl * SECTOR_PER_TRACK + clusterInTrack * SECTORS_PER_CLUSTER);

}
