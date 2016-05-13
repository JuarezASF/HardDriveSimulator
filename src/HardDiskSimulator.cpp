//
// Created by jasf on 5/12/16.
//

#include "HardDiskSimulator.h"
#include <iostream>

using namespace std;

HardDiskSimulator::HardDiskSimulator(track_array *cylinders) {

    if (cylinders == nullptr) {
        cout << "nullptr ptr received!" <<
        endl;
    }

    this-> cylinders = cylinders;
    this->qtdCylinders = qtdCylinders;

}

void HardDiskSimulator::zeroOutCylinders() {
    //not necessary


}

