#include "../inc/procesor.h"

using namespace std;

void resetProcesora() {
    izvrsen_halt = false;
    
    // inicijalizacija memorije
    int tmp = hex_kod.size();
    for (int i = 0; i < tmp; i++) {
        memorija[i] = hex_kod[i];
    }

    // inicijalizacija registara opste namene
    for (int i = 0; i < 9; i++) {
        r[i] = 0;
    }

    // inicijalizacija SP-a
//    r[6] = START_STACK + 2;              // sp = 0x1000;            sp <=> r[6]
    r[8] = 0;                            // 0b000000000000000      psw <=> r[8]

    string startPC = "";            // 0ti ulaz IV tabele je je adresa instrukcije koju na koju PC treba da pokazuje  nakon reseta
    
    // memorija = 10 00 ...         <--- u little endia-nu
    startPC += memorija[2];
    startPC += memorija[3];
    startPC += memorija[0];
    startPC += memorija[1];

    r[7] = konverzija_hexa_u_intE(startPC);     // pc <=> r[7]
}