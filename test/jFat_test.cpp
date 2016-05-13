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

    //use first cluster
    fat->markClusterAsUsed(SectorAddr(0, 0, 0));
    assert(fat->getSectorIdxOfNextFreeCluster() == 4);

    //frer it and see if next free clust if the original one
    fat->markClusterAsFree(SectorAddr(0, 0, 0));
    assert(fat->getSectorIdxOfNextFreeCluster() == 0);

    //mark first two clusters as used
    fat->markClusterAsUsed(SectorAddr(0, 0, 0));
    fat->markClusterAsUsed(SectorAddr(0, 0, 1));
    assert(fat->getSectorIdxOfNextFreeCluster() == 8);

    //free what we've done
    fat->markClusterAsFree(SectorAddr(0, 0, 0));
    fat->markClusterAsFree(SectorAddr(0, 0, 1));

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

    //test for all valid cylinder
    for (unsigned int i = 0; i < QTD_CYLINDERS; i++) {
        try {
            fat->markClusterAsUsed(SectorAddr(i, 0, 0));
            fat->markClusterAsFree(SectorAddr(i, 0, 0));
        } catch (std::runtime_error e) {
            assert(string("There should be no exception here!").length() == 0);
            cout << e.what() << endl;
        }
    }
    //test for all valid tracks
    for (unsigned int i = 0; i < TRACK_PER_CYLINDER; i++) {
        try {
            fat->markClusterAsUsed(SectorAddr(0, i, 0));
            fat->markClusterAsFree(SectorAddr(0, i, 0));
        } catch (std::runtime_error e) {
            assert(string("There should be no exception here!").length() == 0);
            cout << e.what() << endl;
        }
    }

    //test for all valid clustesr
    for (unsigned int i = 0; i < CLUSTERS_PER_TRACK; i++) {
        try {
            fat->markClusterAsUsed(SectorAddr(0, 0, i));
            fat->markClusterAsFree(SectorAddr(0, 0, i));
        } catch (std::runtime_error e) {
            assert(string("There should be no exception here!").length() == 0);
            cout << e.what() << endl;
        }
    }

    //mark entirely first cylinder
    for (unsigned int j = 0; j < TRACK_PER_CYLINDER; j++)
        for (unsigned int i = 0; i < CLUSTERS_PER_TRACK; i++)
            fat->markClusterAsUsed(SectorAddr(0, j, i));

    assert(fat->getSectorIdxOfNextFreeCluster() == 300);

    //undo it
    for (unsigned int j = 0; j < TRACK_PER_CYLINDER; j++)
        for (unsigned int i = 0; i < CLUSTERS_PER_TRACK; i++)
            fat->markClusterAsFree(SectorAddr(0, j, i));

    //mark all first track
    for (unsigned int i = 0; i < CLUSTERS_PER_TRACK; i++)
        fat->markClusterAsUsed(SectorAddr(0, 0, i));

    //after filling first track, of first cylinder, we move to first track of second cylinder
    assert(fat->getSectorIdxOfNextFreeCluster() == 300);

    //undo it
    for (unsigned int i = 0; i < CLUSTERS_PER_TRACK; i++)
        fat->markClusterAsFree(SectorAddr(0, 0, i));

    //after filling first track, of first cylinder, we move to first track of second cylinder
    assert(fat->getSectorIdxOfNextFreeCluster() == 0);
    assert(fat->getSectorIdxOfClusterContinuation(0) == 0);
    fat->markClusterAsUsed(SectorAddr(0,0,0));
    assert(fat->getSectorIdxOfClusterContinuation(0) == 60);

}

