#ifndef _NAKON_PROLAZA_
#define _NAKON_PROLAZA_

#include <iostream>
#include <string>

#include <regex>
#include <sstream>
#include <fstream>
#include <sstream>
#include "simbol.h"
#include "relZapis.h"
#include "flink.h"
#include "regex.h"
#include "glob_podaci.h"

#include <list>
#include <vector>

using namespace std;

void backpatching();

void formiranje_tabele_simbola();

void azuriranje_relZapisa();

void ispis_izlaza_u_fajl();

int dohvati_brRelZapisa_za_sekciju(int red_br_sekcije);

string dodavanje_razmaka(string s);

#endif