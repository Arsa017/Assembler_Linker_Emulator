#ifndef _LINKER_PARSIRANJE_
#define _LINKER_PARSIRANJE_

#include <iostream>
#include <fstream>
#include <algorithm>    // za f-ju std::replace()
#include <string>
#include <regex>

using namespace std;

void obradiFajl(ifstream* ulaz, int red_br_fajla);

void parsirajLiniju(string linija, int red_br_fajla);

void dodaj_u_tabelu_simbola(string linija, int red_br_fajla);

void dodaj_u_tabelu_relZapisa(string linija, int red_br_fajla);

void parsiraj_masinski_kod(string linija, int red_br_fajla);

#endif