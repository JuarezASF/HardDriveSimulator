/***
 *
 * Universidade de Brasília
 *
 * 1/2016
 *
 * Organização de Arquivos
 *
 * Aluno:
 *
 *      Juarez Aires Sampaio Filho
 *      matrícula 11/0032829
 *
 */


#include <iostream>
#include <fstream>
#include "Structs.h"
#include "jFat.h"

using namespace std;


static string valid_extension = ".txt";
bool debugOn = false;

/**
 * Mostra menu
 */
void show_menu() {
    cout << "1 - Escrever Arquivo" << endl;
    cout << "2 - Ler Arquivo" << endl;
    cout << "3 - Apagar Arquivo" << endl;
    cout << "4 - Mostrar Tablea FAT" << endl;
    cout << "5 - Sair" << endl;
    if (debugOn) {
        cout << "6 - Debug print" << endl;

    }
    cout << ">";
}

/***
 * checa se string entrada bate com a extensão de arquivo esperada
 */
bool validateFilename(string fn) {
    if (fn.find(".") == std::string::npos) {
        cout << "Nome de arquivo inválido!" << endl;
        return false;
    }

    string ext = fn.substr(fn.find('.'), 4);
    if (ext.compare(valid_extension) != 0) {
        cout << "Extensão de arquivo inválida!" << ext << endl;
        return false;
    }

    return true;

}

int main() {

    track_array *cylinder = new track_array[QTD_CYLINDERS];

    bool quitRequested = false;
    int option;

    unsigned int fileSize;
    char *fileBytesBuffer = nullptr;
    char readBuffer[BYTES_PER_SECTOR];
    int currentFileBufferSize = 0;
    std::ifstream is;
    std::ofstream os;
    int qtdOfRequiredCluster;
    int qtdWrittenBytes = 0;
    uint sectorsWritten = 0;

    HardDiskSimulator virtualDisk(cylinder, QTD_CYLINDERS);
    jFat *fat = jFat::getInstance();
    fat->setVirtualHardDisk(&virtualDisk);
    string filename;
    double operation_time;

    unsigned int currentStartofClusterSectorAddr;
    unsigned int lastClusterSectorAddr;
    unsigned int currentSectorAddr;
    string outfilename = "SAIDA.txt";
    unsigned short bytesRead = 0;


    while (!quitRequested) {
        show_menu();

        //continua somente com entrada válida
        cin >> option;
        switch (option) {
            case 1:
                cout << "Entre nome do arquivo de texto:" << endl << ">";
                filename.clear();
                cin >> filename;

                if (!validateFilename(filename))
                    continue;

                if (is.is_open())
                    is.close();
                is.open(filename.c_str());
                if (!is.is_open()) {
                    cerr << "Arquivo de entrada " << filename << " não encontrado!" << endl;
                    break;
                }
                //find size of file
                is.seekg(0, std::ios_base::end);
                fileSize = is.tellg();
                is.seekg(0, std::ios_base::beg);

                //increase buffer size if necessary
                if (currentFileBufferSize < fileSize) {
                    if (fileBytesBuffer) {
                        delete[] fileBytesBuffer;
                    }
                    fileBytesBuffer = new char[fileSize];
                    currentFileBufferSize = fileSize;
                }

                //read entire file at once
                is.read(fileBytesBuffer, fileSize);

                qtdOfRequiredCluster = (int) (fileSize / (BYTES_PER_CLUSTER) + 0.5);


                qtdWrittenBytes = 0;
                sectorsWritten = 0;
                lastClusterSectorAddr = INVALID_DISK_POS;


                //tries to allocate fat entry for new file
                currentStartofClusterSectorAddr = fat->getSectorIdxOfNextFreeCluster();
                if (currentStartofClusterSectorAddr == INVALID_DISK_POS) {
                    throw runtime_error("Operacao de achar endereço para novo cluster falho!");
                }
                if (!fat->addFileFirstSector(filename, currentStartofClusterSectorAddr)) {
                    cerr << "Problem ao criar entrada na FAT! Delete o arquivo previamente criado com mesmo nome!" << endl;
                    break;

                }

                int lastSectorInsideClusterWritten;

                // assumes first seek comes from a random position
                operation_time = MEAN_SEEK_TIME;
                virtualDisk.positionHead(SectorAddr::getClusterDetailedAddr(currentStartofClusterSectorAddr));
                while (qtdWrittenBytes < fileSize) {
                    if (lastClusterSectorAddr != INVALID_DISK_POS) {
                        currentStartofClusterSectorAddr = fat->getSectorIdxOfClusterContinuation(
                                currentStartofClusterSectorAddr);
                        fat->setContinuationOfSector(lastClusterSectorAddr + SECTORS_PER_CLUSTER - 1,
                                                     currentStartofClusterSectorAddr);
                    }


                    if (currentStartofClusterSectorAddr == INVALID_DISK_POS) {
                        throw runtime_error("Falha ao procurar por endereço de novo cluster!");
                    }


                    for (int i = 0; i < SECTORS_PER_CLUSTER; i++) {


                        unsigned int qtdToWriteNow = min((unsigned int) 512, fileSize - qtdWrittenBytes);
                        if (qtdToWriteNow == 0) {
                            break;

                        }
                        virtualDisk.writeToSector(fileBytesBuffer + BYTES_PER_SECTOR * (sectorsWritten++),
                                                  qtdToWriteNow, currentStartofClusterSectorAddr + i, &operation_time);

                        qtdWrittenBytes += qtdToWriteNow;
                        lastSectorInsideClusterWritten = i;
                        if (qtdWrittenBytes >= fileSize) {
                            break;

                        }


                    }

                    fat->markClusterAsUsed(currentStartofClusterSectorAddr);
                    lastClusterSectorAddr = currentStartofClusterSectorAddr;


                }

                fat->setSectorAsEOF(lastClusterSectorAddr + lastSectorInsideClusterWritten);


                is.close();

                cout << "Pronto!" << endl;
                cout << "Tempo(virtual) de execução: " << operation_time << " ms" << endl;


                break;
            case 2:
                filename.clear();
                bytesRead = 0;

                cout << "Entre nome do arquivo a ser procurado:" << endl << ">";
                cin >> filename;

                if (!fat->hasFile(filename)) {
                    cerr << "Arquivo nao encontrado!" << endl;
                    break;
                }

                currentSectorAddr = fat->getFirstSectorOfFile(filename);

                if (currentSectorAddr == INVALID_DISK_POS) {
                    break;
                }

                if (os.is_open())
                    os.close();
                os.open(outfilename.c_str(), ios::binary | ios::out | ios::trunc);

                if (!os.is_open()) {
                    cerr << "Arquivo de saida " << outfilename << " nao pode ser aberto!" << endl;
                    break;
                }

                // assumes first seek comes from a random position
                operation_time = MEAN_SEEK_TIME;
                virtualDisk.positionHead(SectorAddr::getClusterDetailedAddr(currentStartofClusterSectorAddr));
                while (true) {
                    if (currentSectorAddr == INVALID_DISK_POS) {

                        cerr << "Posicao invalida!" << endl;
                        break;
                    }
                    virtualDisk.readSector(currentSectorAddr, readBuffer, &bytesRead, &operation_time);

                    os.write(readBuffer, bytesRead);

                    if (fat->isSectorLasfOfFile(currentSectorAddr)) {
                        break;
                    }

                    currentSectorAddr = fat->getContinuationOfSector(currentSectorAddr);

                }

                os.flush();
                os.close();
                cout << "Leitura concluida!" << endl;
                cout << "Saida escrita para: " << outfilename << endl;
                cout << "Tempo de execução: " << operation_time << " ms" << endl;

                break;
            case 3:
                filename.clear();
                bytesRead = 0;

                cout << "Entre nome do arquivo a ser procurado:" << endl << ">";
                cin >> filename;

                if (!fat->hasFile(filename)) {
                    cerr << "Arquivo nao encontrado!" << endl;
                    break;
                }

                currentSectorAddr = fat->getFirstSectorOfFile(filename);

                //marca todos os setores associados ao arquivo como livres
                while ((currentSectorAddr = fat->markClusterAsFree(currentSectorAddr)) != INVALID_DISK_POS) {
                }

                // remove entrada da tabela FAT do arquivo
                fat->removeEntry(filename);

                cout << "Remocao concluida!" << endl;
                break;
            case 4:
                fat->printFatTable();
                break;
            case 5:
                cout << "Encerrando o programa!" << endl;
                quitRequested = true;
                break;
            case 6:
                if (!debugOn) {
                    cout << "Opção inválida!" << endl;
                    break;
                }

                fat->debugPrint();
                break;
            default:
                cout << "Opção inválida!" << endl;
                break;
        }
    }

    delete[] cylinder;
    return 0;
}