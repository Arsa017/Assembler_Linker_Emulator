#ifndef _EMULATOR_PARSIRANJE_
#define _EMULATOR_PARSIRANJE_

#include <iostream>
#include <fstream>
#include <algorithm>    // za f-ju std::replace()
#include <string>
#include <regex>

using namespace std;


void obradiFajl(ifstream* ulaz);

void obradiLiniju(string l);


#endif