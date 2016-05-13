//
// Created by jasf on 5/12/16.
//

#ifndef DISKSIMULATOR_STRUCTS_H
#define DISKSIMULATOR_STRUCTS_H

#define BYTES_PER_SECTOR 512
#define SECTOR_PER_TRACK 60
#define TRACK_PER_CYLINDER 5
#define QTD_CYLINDERS 10
#define SECTORS_PER_CLUSTER 4
#define SECTORS_PER_CYLINDER 300
#define CLUSTERS_PER_TRACK 15
#define INVALID_DISK_POS 0xFFFFFFFF

typedef struct block {
    unsigned char bytes_s[BYTES_PER_SECTOR];
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
    void validate(unsigned int cyl, unsigned int t, unsigned int clu);
public:
    unsigned int cylinder, track, cluster;

    SectorAddr(unsigned int cyl, unsigned int t, unsigned clu);

    SectorAddr();

    static SectorAddr getCTC_fromAbsolute(unsigned int absolute);

    static unsigned int getSectorFromCTC(const SectorAddr &s);

    SectorAddr &operator=(const SectorAddr &A);

    unsigned int getSector() const;


};

#endif //DISKSIMULATOR_STRUCTS_H
