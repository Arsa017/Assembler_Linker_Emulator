#ifndef _OBRADA_LINKER_H_
#define _OBRADA_LINKER_H_

#include <iostream>
#include <fstream>
#include <algorithm>    // za f-ju std::replace()
#include <string>
#include <regex>

using namespace std;

void linkovanje_sekcija();

void azuriranje_globalnih_simbola();

bool nadji_Simbol_u_Finalnoj_tabeli_simbola(string nazivSimbola);

int azuriraj_finalRb_sekcije_za_simbol(int currRbSekcije, int rbFajla);

int odredi_finalnu_vrednost(int currRbSekcje, int rbFajla, int currVrednost);

void azuriranje_rel_zapisa();

int nadji_pomeraj_sekcije(string nazivSekije, int rbFajla);

int rb_simbola_za_rel_zapis(int rbSimbola, int rbFajla);

void povezivanje_masinskog_koda();

void razresavanje_rel_zapisa();

int vrednost_za_ugradnju(int rbSimbola, int rbFajla);

int nadji_vrednost_sekcije(string nazivSekcije, int rbFajla);

int nadji_staru_vrednost(int indeks);

void ugradi_novu_vrednost(int offset, int vrednost);

void ispis_u_izlazni_fajl();

string formatiranje_hex_koda(string str);

#endif