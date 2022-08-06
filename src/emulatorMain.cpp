#include <iostream>
#include <fstream>
#include <algorithm>    // za f-ju std::replace()
#include <string>
#include <regex>

#include "../inc/glob_podaciEmulator.h"
#include "../inc/procesor.h"
#include "../inc/instrukcije.h"
#include "../inc/emulatorParsiranje.h"

using namespace std;



int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Greska! Pokretanje emulatora se zadaje u formi: " << endl;
        cout << "./emulator program.hex" << endl;
        exit(1);
    }   

    string ulazniFajl = argv[1];

    try {
        ulazEmulatora.open("../test/" + ulazniFajl);
    } catch (exception& e) {
        cout << "Ne postoji fajl sa zadatim imenom!" << endl;
        exit(1);
    }

    obradiFajl(&ulazEmulatora);

    resetProcesora();
    
    interpretiranjeInstrukcija();

    ulazEmulatora.close();

    return 0;
}

// kompajlovanje
// g++ emulatorMain.cpp glob_podaciEmulator.cpp procesor.cpp instrukcije.cpp emulatorParsiranje.cpp -o emulator

// pokretanje
// ./emulator program.hex