//
// Created by jasf on 5/12/16.
//

#ifndef DISKSIMULATOR_HARDDISKSIMULATOR_H
#define DISKSIMULATOR_HARDDISKSIMULATOR_H

#include "Structs.h"


class HardDiskSimulator {

private:

    JTrack *cylinders;
    unsigned int qtdCylinders;

    SectorAddr currentHeadPos;

    void zeroOutCylinders();


public:

    HardDiskSimulator(track_array *cylinders, unsigned int qtdCylinders);

    /**
     * Size tem que ser <= 512. O tempo de execução é acrescido em *time
     */
    void writeToSector(char *buffer, unsigned int size, unsigned int sectorIdx, double *time);

    /*
     * Marca quantidades de bytes utilizados para zero
     */
    void clearSector(unsigned int sectorIdx);

    void readSector(unsigned int sectorIdx, char *output_buffer, unsigned short *outputSize, double *time);

    /**
     * Retorna qtd de bytes utilizados naquele setor
     */
    unsigned short getQtdWrittenOnSector(unsigned int sectorIdx);

    /**
     * Seta a posicao atual da cabeca de leitura para o cilindro do endereco informado
     */
    void positionHead(SectorAddr pos);

};


#endif //DISKSIMULATOR_HARDDISKSIMULATOR_H
