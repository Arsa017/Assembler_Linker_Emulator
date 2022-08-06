#ifndef _SIMBOL_H_LINKER_
#define _SIMBOL_H_LINKER_

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include "flink.h"

using namespace std;

class SimbolLinker {

public:

    string naziv;       // naziv simbola
    int sekcija;        // redni broj sekcije u kojoj je simbol definisan!
    int vrednost;       // vrednost Location counter-a na kojoj se simbol/sekcija nalazi
    char vidljivost;    // da li je globalni/lokalni
    int redniBroj;      // redni broj SIMBOLA u tabeli simbola
    int velicina;       // polje za upis velicine sekcije; za labela je 0
    int red_br_fajla;   // redni broj fajla u kome se nalazi simbol
    bool obradjen;      // da li je nakon spajanja sekcija simbol obradjen

    SimbolLinker();

    SimbolLinker(string n, int sek, int vr, char vid, int rb, int vel, int rb_fajla, bool obr);

    

};

#endif