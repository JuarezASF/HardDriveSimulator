#include <iostream>
#include "Structs.h"

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
    while (!quitRequested) {
        show_menu();
        cin >> option;
        switch (option) {
            case 1:
                cout << "Entre nome do arquivo de text:" << endl;
                string filename;
                cin >> filename;

                if(!validateFilename(filename))
                    continue;


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