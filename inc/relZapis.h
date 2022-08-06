#ifndef _RELZAPIS_H_
#define _RELZAPIS_H_

#include <iostream> 
#include <stdio.h>
#include <string>

using namespace std;


class RelZapis {
    
public:

    int offset;             // adresa lokacije ciji sadrzaj Linker prepravlja
    string  tip;            // tip realokacije
    int red_br_simbola;     // redni broj simbola iz tabele simbola na koji se zapis odnosi
    int red_br_sekcije;     // svi zapisi se cuvaju u jednoj listi, pa radi lakseg kasnijeg ispisa
    char vidljivost_simbola;    // zbog azuriranja REDNOG BROJA Globalnih simbola nakon spajanja liste simbola i liste sekcija

    RelZapis();

    RelZapis(int offset, string tip, int rbSimbola, int rbSekcije, char vidljivost_simbola);
    
};

#endif