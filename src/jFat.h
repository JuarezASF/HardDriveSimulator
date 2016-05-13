//
// Created by jasf on 5/12/16.
//

#ifndef DISKSIMULATOR_JFAT_H
#define DISKSIMULATOR_JFAT_H

#include <iostream>
#include <unordered_map>
#include "Structs.h"

using namespace std;


class jFat {

private:
    jFat(unsigned int qtdSectors);

    unsigned int qtdSectors;

    unordered_map<string, unsigned int> fileFirstSectorMap;

    fatEntry *sectorInfo;


public:

    static jFat *getInstance();

    virtual ~jFat();

    /***
     * returns index of sector starting the next free cluster. The sector is not marked as used.
     *
     * If we cannot find a proper sector, returns INVALID_DISK_POS (all ones)
     */
    unsigned int getSectorIdxOfNextFreeCluster() const;

    /***
     * receives idx of start of a cluster and returns where to continue the file on that
     * clust if necessary.
     *
     * Cluster of the same file should be placed in such a way to optimize file recovery.
     */
    unsigned int getSectorIdxOfClusterContinuation(unsigned int sector_idx) const;

    void markClusterAsUsed(const SectorAddr &s);


};


#endif //DISKSIMULATOR_JFAT_H
