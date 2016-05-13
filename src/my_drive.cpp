#include <iostream>
#include <fstream>
#include "Structs.h"
#include "jFat.h"

using namespace std;


static string valid_extension = ".txt";

void show_menu() {
    cout << "1 - Escrever Arquivo" << endl;
    cout << "2 - Ler Arquivo" << endl;
    cout << "3 - Apagar Arquivo" << endl;
    cout << "4 - Mostrar Tablea FAT" << endl;
    cout << "5 - Sair" << endl;
}

bool validateFilename(string fn) {
    /***
     * checa se string entrada bate com a extensão de arquivo esperada
     */
    if (fn.find(".") == std::string::npos) {
        cout << "cannot find file extension!" << endl;
        return false;
    }

    const basic_string<char, char_traits<char>, allocator<void>> &ext = fn.substr(fn.find('.'), 4);
    if (ext.compare(valid_extension) != 0) {
        cout << "invalid extension " << ext << endl;
        return false;
    }

    return true;

}

int main() {

    track_array *cylinder = new track_array[QTD_CYLINDERS];

    bool quitRequested = false;
    int option;

    int fileSize;
    char *fileBytesBuffer = nullptr;
    int currentFileBufferSize = 0;
    std::ifstream is;
    int qtdOfRequiredCluster;
    unsigned int currentSectorAddr;

    jFat *fat = jFat::getInstance();


    while (!quitRequested) {
        show_menu();
        cin >> option;
        switch (option) {
            case 1:
                cout << "Entre nome do arquivo de text:" << endl;
                string filename;
                cin >> filename;

                if (!validateFilename(filename))
                    continue;

                if (is.is_open())
                    is.close();
                is.open(filename.c_str());
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

                qtdOfRequiredCluster = (int) (fileSize/(BYTES_PER_CLUSTER) + 0.5);

                currentSectorAddr = fat->getSectorIdxOfNextFreeCluster();



                break;
            case 2:
                cout << "Not implemented!" << endl;
                break;
            case 3:
                cout << "Not implemented!" << endl;
                break;
            case 4:
                cout << "Not implemented!" << endl;
                break;
            case 5:
                cout << "Encerrando o programa!" << endl;
                quitRequested = true;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
    }

    delete[] cylinder;
    return 0;
}