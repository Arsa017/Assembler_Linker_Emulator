#ifndef _GLOB_PODACI_LINKER_
#define _GLOB_PODACI_LINKER_

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "simbolLinker.h"
#include "relZapisLinker.h"
#include "regex.h"

using namespace std;

extern list<SimbolLinker*> tabela_simbolaL;    
extern list<SimbolLinker*>::iterator iter_simbolL;
extern list<SimbolLinker*>::iterator pomIter_simbolL;

extern list<RelZapisLinker*> lista_relZapisaL;
extern list<RelZapisLinker*>::iterator iter_relZapisL;

extern list<SimbolLinker*>::iterator iter_finalTable;
extern list<SimbolLinker*> final_tabela_simbolaL;    

extern list<SimbolLinker*> lista_undSimbola;
extern list<SimbolLinker*>::iterator iter_undList;

extern int red_br_sekcijeL;
extern string curr_sekcijaL;
extern int red_br_fajlaL;

extern int sizeCnt;
extern int redBrSekcijaFinal;
extern int redBrFinal;
extern string hex_kod;

extern ifstream ulazLinkera;
extern ofstream izlaz;             // output file stream

struct masKodLinker {
    string nazivSekcije;
    string masinskiKod;
    int rb_Fajla;
    int obradjen;
};


extern list<masKodLinker> srdZaIspis;
extern list<masKodLinker>::iterator iter_masinKodL;
extern list<masKodLinker>::iterator pomIterMK;




// globalne f-je za konverziju
string decimal_u_oznaceni_hexaL(int broj);  // decimalni u hexadecimalni
string negativni_wordL(string s);           //  predstava negativne vrednosti na 2 bajta
string little_endian_wordL(string s);       // konverzija u little_endian
int konverzija_hexa_u_intL(string hexaVrednost);

string hex_u_cetiri_bajta(string s);


#endif