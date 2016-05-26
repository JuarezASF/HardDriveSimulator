//
// Created by jasf on 5/12/16.
//

#include "jFat.h"
#include <vector>

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
            for (unsigned int clu = 0; clu < CLUSTERS_PER_TRACK; clu++) {
                current_sector_idx = SECTORS_PER_CYLINDER * cyl + SECTOR_PER_TRACK * t + SECTORS_PER_CLUSTER * clu;
                if (!sectorInfo[current_sector_idx].used) {
                    return current_sector_idx;
                }

            }
        }
    }

    return INVALID_DISK_POS;
}

unsigned int jFat::getSectorIdxOfClusterContinuation(unsigned int sector_idx) const {
    SectorAddr currentAddr = SectorAddr::getClusterDetailedAddr(sector_idx);
    unsigned currentSectorAddr;

    uint cyl = currentAddr.cylinder;
    uint t = currentAddr.trackInsideCylinder;
    uint clu = currentAddr.clusterInsideTrack;


    for (; cyl < QTD_CYLINDERS; cyl++) {
        for (; clu < CLUSTERS_PER_TRACK; clu++) {
            for (; t < TRACK_PER_CYLINDER; t++) {
                currentSectorAddr = t * SECTOR_PER_TRACK + clu * SECTORS_PER_CLUSTER + cyl * SECTORS_PER_CYLINDER;
                if (!(this->sectorInfo[currentSectorAddr].used))
                    return currentSectorAddr;
            }
            t = 0;
        }
        clu = 0;
    }

    return INVALID_DISK_POS;
}

void jFat::markClusterAsUsed(unsigned int addr) {
    if (addr > LAST_VALID_SECTOR_ADDR) {
        throw runtime_error("Invalid addr!");
    }
    if (addr % SECTORS_PER_CLUSTER != 0)
        throw runtime_error("Addr of first sector on clust must be 0 mod 4!");
    for (unsigned int i = 0; i < SECTORS_PER_CLUSTER; i++) {
        if (sectorInfo[addr + i].used)
            throw runtime_error("Sector id " + std::to_string(addr + i) + " is already in use!");
        sectorInfo[addr + i].used = 1;
        if (i != SECTORS_PER_CLUSTER - 1) {
            sectorInfo[addr + i].next = addr + i + 1;
            sectorInfo[addr + i].eof = 0;

        }
    }

}

void jFat::markClusterAsUsed(const SectorAddr &s) {
    markClusterAsUsed(s.sectorInsideDisk);
}

unsigned int jFat::markClusterAsFree(const SectorAddr &s) {
    return markClusterAsFree(s.sectorInsideDisk);

}

unsigned int jFat::markClusterAsFree(unsigned int addr) {
    if (addr % SECTORS_PER_CLUSTER != 0)
        throw runtime_error("Addr of first sector on clust must be 0 mod 4!");

    uint out = (sectorInfo[addr + SECTORS_PER_CLUSTER - 1].eof) ? INVALID_DISK_POS : sectorInfo[addr +
                                                                                                SECTORS_PER_CLUSTER -
                                                                                                1].next;
    for (unsigned int i = 0; i < SECTORS_PER_CLUSTER; i++) {
        if (!sectorInfo[addr + i].used)
            throw runtime_error("Sector id " + std::to_string(addr + i) + " is not in use! Why you're freeing it?!");
        sectorInfo[addr + i].used = 0;
        sectorInfo[addr + i].eof = 0;
        sectorInfo[addr + i].next = 0;
        virtualDisk->clearSector(addr + i);
    }

    return out;

}


void jFat::debugPrint() {
    for (uint cyl = 0; cyl < QTD_CYLINDERS; cyl++) {
        cout << "cylinder " << cyl << endl;
        for (uint t = 0; t < TRACK_PER_CYLINDER; t++) {
            cout << "\ttrack " << t << endl;
            for (uint sec = 0; sec < SECTOR_PER_TRACK; sec++) {
                uint sector_id = cyl * SECTORS_PER_CYLINDER + t * SECTOR_PER_TRACK + sec;
                cout << "\t s" << sec
                << " u:" << sectorInfo[sector_id].used
                << " n:" << sectorInfo[sector_id].next
                << " e:" << sectorInfo[sector_id].eof
                << " q:" << virtualDisk->getQtdWrittenOnSector(sector_id)
                << endl;
            }

        }

    }

}


void jFat::setContinuationOfSector(const SectorAddr &s, unsigned int continuationAddr) {
    unsigned int addr = s.sectorInsideDisk;
    setContinuationOfSector(addr, continuationAddr);

}

void jFat::setContinuationOfSector(unsigned int addr, unsigned int continuationAddr) {
    if (addr % SECTORS_PER_CLUSTER != SECTORS_PER_CLUSTER - 1)
        throw runtime_error("You can only manualy set sector continuation if sector is last sector of a cluster!");

    sectorInfo[addr].next = continuationAddr;
    sectorInfo[addr].eof = 0;

}

bool jFat::addFileFirstSector(string filename, unsigned int firstSectorAddr) {
    if (fileFirstSectorMap.find(filename) == fileFirstSectorMap.end()) {
        fileFirstSectorMap.insert(make_pair(filename, firstSectorAddr));
        return true;
    }
    else {
        cerr << "There is already an entry for file named: " << filename << endl;
        return false;
    }

}

bool jFat::hasFile(string filename) {
    return fileFirstSectorMap.find(filename) != fileFirstSectorMap.end();
}

void jFat::setSectorAsEOF(unsigned int addr) {
    sectorInfo[addr].eof = 1;

    uint k = addr % SECTORS_PER_CLUSTER;

    while(k != SECTORS_PER_CLUSTER ){
        sectorInfo[k].eof = 1;
        sectorInfo[k].next= INVALID_DISK_POS;
        k++;
    }

}

unsigned int jFat::getFirstSectorOfFile(string filename) {
    if (fileFirstSectorMap.find(filename) == fileFirstSectorMap.end()) {
        cerr << "Cannot find entry for " << filename << endl;
        return INVALID_DISK_POS;
    }

    return fileFirstSectorMap[filename];


}

unsigned int jFat::getContinuationOfSector(unsigned int addr) {
    if (!checkAddr(addr))
        return INVALID_DISK_POS;

    if (!sectorInfo[addr].used) {
        cerr << "Why are you trying to get continuation of a non used sector???" << endl;
        return INVALID_DISK_POS;
    }
    if (sectorInfo[addr].eof) {
        return sectorInfo[addr].next;
    }
    return sectorInfo[addr].next;
}

bool jFat::isSectorLasfOfFile(unsigned int addr) {
    if (!sectorInfo[addr].used) {
        cerr << "How come you ask if a not used sector is EOF???" << endl;
        return false;
    }

    return sectorInfo[addr].eof > 0;
}

void jFat::printFatTable() {

    cout << "NOME\t\t\tTAMANHO EM DISCO\t\t LOCALIZACAO" << endl;

    for (auto it : fileFirstSectorMap) {
        cout << it.first << '\t';

        //collect sectors

        vector<unsigned int> out;
        unsigned int currentSector = it.second;

        while (true) {
            out.push_back(currentSector);

            if (this->isSectorLasfOfFile(currentSector))
                break;

            currentSector = getContinuationOfSector(currentSector);


        }

        uint k = currentSector % SECTORS_PER_CLUSTER;
        if (k != SECTORS_PER_CLUSTER){
            k++;
            while (k != SECTORS_PER_CLUSTER) {
                out.push_back(currentSector + k);
                k++;
            }

        }


        cout << out.size() * BYTES_PER_SECTOR << "\t\t\t\t\t";

        for (uint k = 0; k < out.size() - 1; k++)
            cout << out[k] << ',';
        cout << out[out.size() - 1] << endl;


    }

}

bool jFat::checkAddr(unsigned int addr) {
    if (addr == INVALID_DISK_POS)
        return false;

    if (addr > LAST_VALID_SECTOR_ADDR)
        return false;

    return true;
}

void jFat::removeEntry(string filename) {

    if(fileFirstSectorMap.find(filename) == fileFirstSectorMap.end()){
        cerr << "cannot find file " << filename << " on FAT table" << endl;
    }

    fileFirstSectorMap.erase(filename);

}
