#ifndef _GLOB_PODACI_EMULATOR_
#define _GLOB_PODACI_EMULATOR_

#include <string>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

extern ifstream ulazEmulatora;

extern string hex_kod; 

// memorija procesora je velicine 2^16 bajtova; ali u ovom slucaju zbog skaliranja pomeraja ce biti 2 * 2^16 bajtova
extern char memorija[131072];   // 2 * 65536

extern unsigned short r[9];       // r0, r1, ... r8

extern short pc;                // pc <=> registri[7]
extern short sp;                // sp <=> registri[6]
extern short psw;               // psw <=> registri[8]

extern bool izvrsen_halt;

extern int START_STACK;

extern short FLAG_Z;
extern short FLAG_O;
extern short FLAG_C;
extern short FLAG_N;

// OC-kod za asemblerske naredbe sirine 1B
extern string HALT;
extern string IRET;
extern string RET;

// OC-kod za asemblerske naredbe sirine 2B
extern string INT;
extern string XCHG;
extern string ADD;
extern string SUB;
extern string MUL;
extern string DIV;
extern string CMP;
extern string NOT;
extern string AND;
extern string OR;
extern string XOR;
extern string TEST;
extern string SHL;
extern string SHR;

// OC-kod za asemblerske naredbe sirine 3B ili 5B
extern string CALL;
extern string JMP;
extern string JEQ;
extern string JNE;
extern string JGT;
// extern string PUSH;
// extern string POP;
extern string LDR;
extern string STR;

extern int IMMED;
extern int REGDIR;
extern int PCREL;
extern int REGIND;
extern int REGINDPOM;
extern int MEM;

// globalne f-je za konverziju
string decimal_u_oznaceni_hexaE(int broj);  // decimalni u hexadecimalni
string negativni_wordE(string s);           //  predstava negativne vrednosti na 2 bajta
string little_endian_wordE(string s);       // konverzija u little_endian
int konverzija_hexa_u_intE(string hexaVrednost);

string hex_u_cetiri_bajtaE(string s);

int broj_registra(string s);

int dohvati_staru_vrednost(int indeks);

void dest_src_regIndex(int* dest, int* src);

void smestanje_u_memoriju(short index, short val);

string vrednost_u_registru_za_prikaz(int val);

#endif