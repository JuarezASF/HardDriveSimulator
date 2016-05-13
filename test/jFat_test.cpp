//
// Created by jasf on 5/12/16.
//

# include <cassert>
# include <string>


#include "../src/jFat.h"

int main() {

    jFat *fat = jFat::getInstance();

    for (unsigned int i = 0; i < 10; i++)
        assert(fat->getSectorIdxOfNextFreeCluster() == 0);

    fat->markClusterAsUsed(SectorAddr(0, 0, 0));
    assert(fat->getSectorIdxOfNextFreeCluster() == 4);

    fat->markClusterAsUsed(SectorAddr(0, 0, 1));
    assert(fat->getSectorIdxOfNextFreeCluster() == 8);

    //test for invalid cylinder
    try {
        fat->markClusterAsUsed(SectorAddr(QTD_CYLINDERS, 0, 0));
        assert(string("There should be an exception here!").length() == 0);
    } catch (std::runtime_error e) {
    }

    //test for invalid track
    try {
        fat->markClusterAsUsed(SectorAddr(0, TRACK_PER_CYLINDER, 0));
        assert(string("There should be an exception here!").length() == 0);
    } catch (std::runtime_error e) {
    }

    //test for invalid cluster
    try {
        fat->markClusterAsUsed(SectorAddr(0, 0, CLUSTERS_PER_TRACK));
        assert(string("There should be an exception here!").length() == 0);
    } catch (std::runtime_error e) {
    }

    //test for valid cylinder

    for (unsigned int i = 0; i < QTD_CYLINDERS; i++) {
        try {
            fat->markClusterAsUsed(SectorAddr(i, 0, 0));

        } catch (std::runtime_error e) {
//            assert(string("There should be no exception here!").length() == 0);
            cout << e.what() << endl;
        }
    }


}

