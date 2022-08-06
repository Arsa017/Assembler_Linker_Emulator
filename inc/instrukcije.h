#ifndef _INSTRUKCIJE_
#define _INSTRUKCIJE_

#include <iostream>
#include <fstream>
#include <algorithm>    // za f-ju std::replace()
#include <string>

#include "../inc/glob_podaciEmulator.h"

using namespace std;

void interpretiranjeInstrukcija();

void push(short val);

short pop();


short skidanje_sa_steka(int index);

void smestanje_na_stek(int index, short val);

// jednobajnte instrukcije
void obradi_halt();
void obradi_iret();
void obradi_ret();

// dvobajnte instrukcije
void obradi_int();
void obradi_xchg();
void obradi_add();
void obradi_sub();
void obradi_mul();
void obradi_div();
void obradi_not();
void obradi_and();
void obradi_or();
void obradi_xor();
void obradi_test();
void obradi_shr();
void obradi_shl();
void obradi_cmp();

// instrukcije od 3 ili 5 bajta
void obradi_ldr();   
void obradi_str();

// instrukcije skoka
void obradi_jmp();
void obradi_jeq();
void obradi_jne();
void obradi_jgt();
void obradi_call();

int dohvati_operand(int regD, int regS, int update, int adresiranje);

void setovanje_flegova(string oc, short value1, short value2);

int operand_instrukcije_skoka();

void ispis_stanja_procesora();

#endif