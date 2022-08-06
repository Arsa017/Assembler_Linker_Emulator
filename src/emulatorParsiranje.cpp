#include "../inc/emulatorParsiranje.h"
#include "../inc/glob_podaciEmulator.h"
#include <string>
#include <regex>
#include <algorithm>    // za f-ju std::replace()

using namespace std;

void obradiFajl(ifstream* ulaz) {
    int brojLinije = 0;
    string linija;

    while(getline(*ulaz, linija)) {
        
        if (brojLinije < 1) {
            brojLinije++;                                  // preskakanje prve linije u fajlu posto je prazna
            continue;
        }

        obradiLiniju(linija);
        brojLinije++;
    }

    hex_kod.erase(remove(hex_kod.begin(), hex_kod.end(), ' '), hex_kod.end());      // brisanje praznih znakova u hex_kodu;  12 34 56 ... ==> 123456...
}

void obradiLiniju(string linija) {
    regex HEX("^\\w{4}\\:");
    linija = regex_replace(linija, HEX, "");                     // 0000: 12 34 56 ... ==> 12 34 56 ...
    linija = regex_replace(linija, regex("^\\s+"), "");          // brisanje praznog znaka sa pocetka stringa; ==>12 34 56 ...
    hex_kod += linija;
}
