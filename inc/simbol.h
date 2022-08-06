#ifndef _SIMBOL_H_
#define _SIMBOL_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include "flink.h"

using namespace std;

class Simbol {

public:

    string naziv;       // naziv simbola
    int sekcija;        // redni broj sekcije u kojoj je simbol definisan!
    int vrednost;       // vrednost Location counter-a na kojoj se simbol/sekcija nalazi
    char vidljivost;    // da li je globalni/lokalni
    int redniBroj;      // redni broj SIMBOLA u tabeli simbola
    int velicina;       // polje za upis velicine sekcije; za labela je 0

    bool definisan;         // da li je simbol definisan; TRUE kad se naidje na def. simbola   
    bool extrn; 
    list<Flink*> lista;     // lista zapisa za Backpatching 

    Simbol();

    Simbol(string n, int sek, int vr, char vid, int rb, int vel, bool def, bool extrn);

    void dodaj(Flink* zapis);

};

#endif