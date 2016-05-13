#include <stdexcept>
#include "Structs.h"
#include "exception"
#include "string"
SectorAddr::SectorAddr(unsigned int cyl, unsigned int t, unsigned clu) : cylinder(cyl), track(t), cluster(clu) {
    validate(cyl, t, clu);

}

SectorAddr::SectorAddr() : cylinder(0), track(0), cluster(0) {
}

SectorAddr SectorAddr::getCTC_fromAbsolute(unsigned int absolute) {
    unsigned int cyl, t, clu;
    absolute /= 4;
    clu = absolute % 15;
    absolute -= clu;
    absolute /= 15;
    t = absolute % 5;
    cyl = (absolute - t) / 5;
    return SectorAddr(cyl, t, clu);
}

unsigned int SectorAddr::getSectorFromCTC(const SectorAddr &s) {
    return SECTORS_PER_CYLINDER * s.cylinder + s.track * SECTOR_PER_TRACK + s.cluster * SECTORS_PER_CLUSTER;
}


SectorAddr &SectorAddr::operator=(const SectorAddr &A) {
    validate(A.cylinder, A.track, A.cluster);
    cylinder = A.cylinder;
    track = A.track;
    cluster = A.cluster;
    return *this;
}

unsigned int SectorAddr::getSector() const {
    return SectorAddr::getSectorFromCTC(*this);
}

void SectorAddr::validate(unsigned int cyl, unsigned int t, unsigned int clu) {
    if (clu >= CLUSTERS_PER_TRACK)
        throw std::runtime_error("Invalid cluster idx:" + std::to_string(clu));

    if (t >= TRACK_PER_CYLINDER)
        throw std::runtime_error("Invalid track idx:" + std::to_string(t));

    if (cyl >= QTD_CYLINDERS)
        throw std::runtime_error("Invalid cylinder idx:" + std::to_string(cyl));

}
