#ifndef _GLOB_PODACI_H_
#define _GLOB_PODACI_H_

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "simbol.h"
#include "relZapis.h"
#include "flink.h"
#include "regex.h"

using namespace std;

extern ifstream ulaz;
extern ofstream izlaz;

extern int brojacLinije;

extern string linija;
extern string curr_sekcija;
extern int loc_counter;
extern int red_br_sekcije;
extern int red_br_simbola;

// za svaku sekciju imamo prazan string u kome cemo pamtiti njen masinski kod 
struct masinKod {
    string nazivSekcije;
    string masinskiKod;
};

extern list<Simbol*> tabela_simbola;    // na kraju prolaza prvo smestimo sve sekcije, a zatim sve simbole ciji je redBr uvecan za broj sekcija, kako bi se ocuvao redosled
extern list<Simbol*> lista_sekcija;
extern list<Simbol*> lista_simbola;
extern list<Simbol*>::iterator iter_simbol;

extern list<RelZapis*> lista_relZapisa;
extern list<RelZapis*>::iterator iter_relZapis;

extern list<Flink*>::iterator iter_flink;

extern vector<masinKod> sadrzajZaIspis;
extern vector<masinKod>::iterator iter_masinKod;


// globalne f-je za konverziju
string decimal_u_oznaceni_hexa(int broj);  // decimalni u hexadecimalni
string negativni_word(string s);           //  predstava negativne vrednosti na 2 bajta
string little_endian_word(string s);       // konverzija u little_endian

// globalne f-je za odredjivanje broja registra;
int vrati_broj_registra(string s);

string odredi_DestSrc_bajt(vector<string> niz, string instr);

vector<string> vrati_niz_Registra_Iz_Linije(string linija);

int konverzija_hexa_u_int(string hexaVrednost);

#endif