//
// Created by jasf on 5/12/16.
//

#ifndef DISKSIMULATOR_JFAT_H
#define DISKSIMULATOR_JFAT_H

#include <iostream>
#include <unordered_map>
#include "Structs.h"
#include "HardDiskSimulator.h"

using namespace std;


class jFat {

private:
    HardDiskSimulator *virtualDisk;
    jFat(unsigned int qtdSectors);

    unsigned int qtdSectors;

    unordered_map<string, unsigned int> fileFirstSectorMap;

    fatEntry *sectorInfo;

    bool checkAddr(unsigned int addr);


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

    /**
     * A SectorAddr is received. The addr must be the beggining of a cluster. We set the sector
     * on this addr and the next 4
     */
    void markClusterAsUsed(unsigned int sectorIdx);
    void markClusterAsUsed(const SectorAddr &s);

    unsigned int markClusterAsFree(const SectorAddr &s);
    unsigned int markClusterAsFree(unsigned int addr);

    void setContinuationOfSector(const SectorAddr &s, unsigned int continuationAddr);
    void setContinuationOfSector(unsigned  int clusterIdx, unsigned int continuationAddr);

    void setSectorAsEOF(unsigned  int clusterIdx);

    void debugPrint();

    void printFatTable();

    /**
     * Para criar nova entrada na tabela
     */
    bool addFileFirstSector(string filename, unsigned int firstSectorAddr);

    /**
     * Indica se entrada com aquele nome já existe na tabela
     */
    bool hasFile(string filename);

    /**
     * Retorna primeiro setor associado ao arquivo
     */
    unsigned int getFirstSectorOfFile(string filename);

    /**
     * Retorna o campo next to sextor informado
     */
    unsigned int getContinuationOfSector(unsigned int addr);

    /**
     * Retorna verdadeiro caso o setor seja o último de um arquivo
     */
    bool isSectorLasfOfFile(unsigned int addr);


    inline void setVirtualHardDisk(HardDiskSimulator *ptr){ virtualDisk = ptr;}

    /**
     * Usado para remover entradas associadas a um arquivo. Isso por si só não marca esses setores como livre!
     */
    void removeEntry(string filename);




};


#endif //DISKSIMULATOR_JFAT_H
