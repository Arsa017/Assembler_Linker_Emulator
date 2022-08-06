#ifndef _OBRADA_LINIJE_H_
#define _OBRADA_LINIJE_H_

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

void obrada_linije();    // liniju smo sacuvali kao globalni podatak

void obrada_global(string linija);

void obradi_extern(string linija);

void obrada_sekcije(string linija);

void obrada_skip(string linija);

void obrada_end(string linija);

void obrada_word();

void obrada_labele(string linija);

void obrada_asemblerske_naredbe();

void obrada_jednoBajtne_naredbe();   // asemblerske naredbe sirine 1B

void obrada_dvoBajtne_naredbe();     // asemblerske naredbe sirine 2B
//--------------------------------------------------------------
void obradi_instrukciju_int();
void obradi_instrukciju_xchg();
void obradi_instrukciju_add();
void obradi_instrukciju_sub();
void obradi_instrukciju_mul();
void obradi_instrukciju_div();
void obradi_instrukciju_cmp();
void obradi_instrukciju_not();
void obradi_instrukciju_and();
void obradi_instrukciju_or();
void obradi_instrukciju_xor();
void obradi_instrukciju_test();
void obradi_instrukciju_shl();
void obradi_instrukciju_shr();

void obrada_naredbe_sirine_3_ili_5_bajta();     // asemblerske naredbe sirine 3 ili 5 bajta
//--------------------------------------------------------------
void obrada_instrukcije_push();
void obrada_instrukcije_pop();
void obrada_instrukcije_ldr();
void obrada_instrukcije_str();
void obrada_instrukcije_jmp();
void obrada_instrukcije_jeq();
void obrada_instrukcije_jne();
void obrada_instrukcije_jgt();
void obrada_instrukcije_call();

void obrada_operanda_za_vrednost_podatka(string operand);       //  operand koji se koristi kao vrednost podatka u instrukciji
//--------------------------------------------------------------
void obrada_literala(string operand);   // neposredno adresiranje
void obrada_simbola(string operand);    // neposredno adresiranje
void obrada_memlit(string operand);     // memorijsko direktno adresiranje
void obrada_regdir(string operand);     // registarsko direktno adresiranje
void obrada_memsim(string operand);     // memorijsko direktno adresiranje
void obrada_mempcsim(string operand);   // vrednost iz memorije PCrelativnim adresiranjem
void obrada_memreg(string operand);     // registarsko indirektno adresiranje
void obrada_memreglit(string operand);  // registarsko indirektno sa pomerajem
void obrada_memregsim(string operand);  // registarsko indirektno sa pomerajem

void obrada_operanda_za_instrukciju_skoka(string operand);      // operand koji se koristi kao odredisna adresa instrukcije skoka
//--------------------------------------------------------------
void obrada_jmp_lit(string operand);        // neposredno adresiranje
void obrada_jmp_sim(string operand);        // neposredno adresiranje
void obrada_jmp_pcsim(string operand);      // PC-relativno adresiranje
void obrada_jmp_memlit(string operand);     // memorijsko direktno adresiranje
void obrada_jmp_regdir(string operand);     // registarsko direktno adresiranje
void obrada_jmp_memsim(string operand);     // memorijsko direktno adresiranje
void obrada_jmp_memreg(string operand);     // registarsko indirektno adresiranje
void obrada_jmp_memreglit(string operand);  // registarsko indirektno sa pomerajem
void obrada_jmp_memregsim(string operand);  // registarsko indirektno sa pomerajem

#endif