#ifndef _FLINK_H_
#define _FLINK_H_

#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

class Flink {
    
public:
    
    int pozicija;   // adresa pozicije za prepravku -> vrednost location countera u trenutku kreiranja strukture
    int sekcija;    // redni broj sekcije u kojoj se koristi dati simbol, a ne u kojoj se taj simbol nalazi; posto nam svaka sekcija pocinje od NULE tj. nema nadovezivanja
    int operacija;  // 0 - sabiranje; 1 - oduzimanje;   default_value = -1;
    int br_bajtova; // broj bajtova za smestanje operanda

    Flink();

    Flink(int poz, int sek, int operac, int brBajt);

  //  friend ostream operator<<(ostream& it, const Flink& f) {
        // return it << f.pozicija << " " << f.sekcija << " " << f.operacija << " " << f.br_bajtova << " " << endl;
  //  }

};

#endif