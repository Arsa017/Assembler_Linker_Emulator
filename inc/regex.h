#ifndef _REGEX_H_
#define _REGEX_H_

#include <regex>
using namespace std;

// regexi za asemblerske direktive
extern regex GLOBAL;
extern regex EXTERN;
extern regex SECTION;
extern regex WORD;
extern regex SKIP;
extern regex END;

extern regex LABELA;

// nula ili vise praznik znakova na pocetku
extern regex BELI_ZNAK;

// regexi za asemblerske naredbe sirine 1B
extern regex HALT;
extern regex IRET;
extern regex RET;

// regexi za asemblerske naredbe sirine 2B
extern regex INT;
extern regex XCHG;
extern regex ADD;
extern regex SUB;
extern regex MUL;
extern regex DIV;
extern regex CMP;
extern regex NOT;
extern regex AND;
extern regex OR;
extern regex XOR;
extern regex TEST;
extern regex SHL;
extern regex SHR;

// regexi za asemblerske naredbe sirine 3B ili 5B
extern regex CALL;
extern regex JMP;
extern regex JEQ;
extern regex JNE;
extern regex JGT;
extern regex PUSH;
extern regex POP;
extern regex LDR;
extern regex STR;

// regexi za operand kojim se definise vrednost podatka
extern regex LITERAL;
extern regex SIM;
extern regex MEMLIT;
extern regex REGDIR;
extern regex MEMSIM;
extern regex MEMPCSIM;
extern regex MEMREG;
extern regex MEMREGLIT;
extern regex MEMREGSIM;

// regexi za operand kojim se definise odrediste skoka
extern regex JMP_LITERAL;
extern regex JMP_SIMBOL;
extern regex JMP_PCSIM;
extern regex JMP_MEMLIT;
extern regex JMP_REGDIR;
extern regex JMP_MEMSIM;
extern regex JMP_MEMREG;
extern regex JMP_MEMREGLIT;
extern regex JMP_MEMREGSIM;

// regexi za linker
extern regex SIMBOL;
extern regex REL_ZAPIS;
extern regex ZVEZDICA;
extern regex RelZapis_SEKCIJA;
extern regex RelZapis_HEADER;

#endif