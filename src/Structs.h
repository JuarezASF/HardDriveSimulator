//
// Created by jasf on 5/12/16.
//

#ifndef DISKSIMULATOR_STRUCTS_H
#define DISKSIMULATOR_STRUCTS_H

typedef unsigned int uint;

#define BYTES_PER_SECTOR (unsigned int)512
#define SECTOR_PER_TRACK (unsigned int)60
#define SECTORS_PER_CLUSTER (unsigned int)4
#define TRACK_PER_CYLINDER (unsigned int)5
#define QTD_CYLINDERS (unsigned int)4

#define SECTORS_PER_CYLINDER (unsigned int)((SECTOR_PER_TRACK)*(TRACK_PER_CYLINDER))
#define CLUSTERS_PER_TRACK (unsigned int)(SECTOR_PER_TRACK)/(SECTORS_PER_CLUSTER)
#define SECTORS_PER_DISK (uint)(SECTORS_PER_CYLINDER)*(QTD_CYLINDERS)

#define BYTES_PER_CLUSTER (uint)(SECTORS_PER_CLUSTER)*(BYTES_PER_SECTOR)
#define BYTES_PER_TRACK (uint)(BYTES_PER_CLUSTER)*(CLUSTERS_PER_TRACK)
#define BYTES_PER_CYLINDER (uint)(BYTES_PER_TRACK)*(TRACK_PER_CYLINDER)
#define BYTES_PER_DISK (uint)(BYTES_PER_CYLINDER)*(QTD_CYLINDERS)

#define LAST_VALID_SECTOR_ADDR (uint)((SECTORS_PER_DISK)-1)
#define INVALID_DISK_POS ((uint)0xFFFFFFFF)

typedef struct block {
    unsigned char bytes_s[BYTES_PER_SECTOR];
    unsigned short qtdOfUsedBytes;
} block;

typedef struct sector_array {
    block sector[SECTOR_PER_TRACK];
} sector_array;
typedef sector_array JSector;

typedef struct track_array {
    sector_array track[TRACK_PER_CYLINDER];
} track_array;
typedef track_array JTrack;

typedef struct fatlist_s {
    char file_name[100];
    unsigned int first_sector;
} fatlist;

typedef struct fatent_s {
    unsigned int used;
    unsigned int eof;
    unsigned int next;

} fatent;
typedef fatent fatEntry;

class SectorAddr {
private:
    SectorAddr();

public:
    unsigned int cylinder;
    unsigned int trackInsideCylinder ;
    unsigned int sectorInsideTrack;
    unsigned int clusterInsideTrack;
    unsigned int sectorInsideCluster;
    unsigned int sectorInsideCylinder;
    unsigned int sectorInsideDisk;

    SectorAddr(uint cyl, uint trackInCyl, uint clusterInTrack);

    static SectorAddr getClusterDetailedAddr(uint absolute);

    SectorAddr &operator=(const SectorAddr &A);


};

#endif //DISKSIMULATOR_STRUCTS_H
