//
// Created by jasf on 5/12/16.
//

#include "jFat.h"

jFat *jFat::getInstance() {
    static jFat *instance = new jFat(QTD_CYLINDERS * TRACK_PER_CYLINDER * SECTOR_PER_TRACK);
    return instance;
}

jFat::~jFat() {
    if (sectorInfo)
        delete[] sectorInfo;

}

jFat::jFat(unsigned int qtdSectors) {

    this->qtdSectors = qtdSectors;

    this->sectorInfo = new fatEntry[qtdSectors];

    for (unsigned int k = 0; k < qtdSectors; k++) {
        sectorInfo[k].used = 0;
    }
}

unsigned int jFat::getSectorIdxOfNextFreeCluster() const {

    unsigned int current_sector_idx;

    for (unsigned int t = 0; t < TRACK_PER_CYLINDER; t++) {
        for (unsigned int cyl = 0; cyl < QTD_CYLINDERS; cyl++) {
            for (unsigned int clu = 0; clu < SECTORS_PER_CLUSTER; clu++) {
                current_sector_idx = SECTORS_PER_CYLINDER * cyl + SECTOR_PER_TRACK*t + SECTORS_PER_CLUSTER*clu;
                if (!sectorInfo[current_sector_idx].used) {
                    return current_sector_idx;
                }

            }
        }
    }

    return INVALID_DISK_POS;
}

unsigned int jFat::getSectorIdxOfClusterContinuation(unsigned int sector_idx) const {
    SectorAddr currentAddr = SectorAddr::getCTC_fromAbsolute(sector_idx);
    unsigned currentSectorAddr;

    unsigned int cyl = currentAddr.cylinder;
    unsigned int t = currentAddr.track;
    unsigned int clu = currentAddr.cluster;


    for (; cyl < QTD_CYLINDERS; cyl++) {
        for (; clu < CLUSTERS_PER_TRACK; clu++) {
            for (; t < TRACK_PER_CYLINDER; t++) {
                currentSectorAddr = SectorAddr::getSectorFromCTC(SectorAddr(cyl, t, clu));
                if (!this->sectorInfo[currentSectorAddr].used)
                    return currentSectorAddr;
            }
            t = 0;
        }
        clu = 0;
    }

    return INVALID_DISK_POS;
}

void jFat::markClusterAsUsed(const SectorAddr &s) {
    unsigned int addr = s.getSector();

    if (addr % 4 != 0)
        throw runtime_error("Addr of first sector on clust must be 0 mod 4!");
    for (unsigned int i = 0; i < SECTORS_PER_CLUSTER; i++) {
        if (sectorInfo[addr + i].used)
            throw runtime_error("Sector id " + std::to_string(addr + i) + " is already in use!");
        sectorInfo[addr + i].used = 1;
    }

}

void jFat::markClusterAsFree(const SectorAddr &s){
    unsigned int addr = s.getSector();

    if (addr % 4 != 0)
        throw runtime_error("Addr of first sector on clust must be 0 mod 4!");
    for (unsigned int i = 0; i < SECTORS_PER_CLUSTER; i++) {
        if (!sectorInfo[addr + i].used)
            throw runtime_error("Sector id " + std::to_string(addr + i) + " is not in use! Why you're freeing it?!");
        sectorInfo[addr + i].used = 0;
    }

}

void jFat::debugPrint() {
    for (unsigned int cyl = 0; cyl < QTD_CYLINDERS; cyl++){
        cout << "cylinder " << cyl << endl;
        for(unsigned int t = 0; t < TRACK_PER_CYLINDER; t++){
            cout << "\ttrack " << t << endl;
            for(unsigned int sec = 0; sec < SECTOR_PER_TRACK; sec++){
                unsigned int sector_id = cyl * SECTORS_PER_CYLINDER + t * SECTOR_PER_TRACK + sec;
                cout << "\t sector" << sec << " used:" << sectorInfo[sector_id].used << endl;
            }

        }

    }

}
