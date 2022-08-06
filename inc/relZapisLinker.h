#ifndef _RELZAPIS_H_LINKER_
#define _RELZAPIS_H_LINKER_

#include <iostream> 
#include <stdio.h>
#include <string>

using namespace std;


class RelZapisLinker {
    
public:

    int offset;                 // adresa lokacije ciji sadrzaj Linker prepravlja
    string  tip;                // tip realokacije
    int red_br_simbola;         // redni broj simbola iz tabele simbola na koji se zapis odnosi
    string nazivSekcije;        // naziv sekcije ciji o cijem sadrzaju RelZapis cuva informaciju; ovo polje je zamena za red_br_sekcije  
    int red_br_fajla;           // red br. fajla u kome se nalazi rel zapis
    int obradjen;              // za slucaj da je potrebno azurirati rel zapis, default vrednost 0

    RelZapisLinker();
    RelZapisLinker(int offset, string tip, int rbSimbola, string nazivSekcije, int red_br_fajla, int obradjen);
    
};

#endif