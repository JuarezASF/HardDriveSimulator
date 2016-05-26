//
// Created by jasf on 5/26/16.
//

typedef unsigned int uint;

#include <Structs.h>
#include <assert.h>
#include <iostream>


using namespace std;
int main() {

    for (uint cylinder = 0; cylinder < QTD_CYLINDERS; cylinder++)
        for (uint t = 0; t < TRACK_PER_CYLINDER; t++) {
            for (uint s = 0; s < SECTOR_PER_TRACK; s++) {
                uint sectorIdx = cylinder * SECTORS_PER_CYLINDER + t * SECTOR_PER_TRACK + s;
                SectorAddr addr = SectorAddr::getClusterDetailedAddr(sectorIdx);

                assert(addr.sectorInsideDisk == sectorIdx);
                assert(addr.sectorInsideCluster == sectorIdx % 4);
                assert(addr.sectorInsideTrack == sectorIdx% 60);
                assert(addr.sectorInsideCylinder == sectorIdx % 300);

                assert(addr.cylinder == cylinder);
                assert(addr.trackInsideCylinder == t);
                assert(addr.sectorInsideTrack == s);
            }
        }

}

