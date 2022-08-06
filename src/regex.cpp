#include "regex.h"

#include <regex>
using namespace std;

// regexi za asemblerske direktive
regex GLOBAL("\\.global");
regex EXTERN("\\.extern");
regex SECTION("\\.section");
regex WORD("\\.word");
regex SKIP("\\.skip");
regex END("\\.end");

regex LABELA("\\w+:");

regex BELI_ZNAK("^\\s+");     // nula ili vise praznik znakova na pocetku

// regexi za asemblerske naredbe sirine 1B
regex HALT("^halt");
regex IRET("^iret");
regex RET("^ret");

// regexi za asemblerske naredbe sirine 2B
regex INT("^int");
regex XCHG("^xchg");
regex ADD("^add");
regex SUB("^sub");
regex MUL("^mul");
regex DIV("^div");
regex CMP("^cmp");
regex NOT("^not");
regex AND("^and");
regex OR("^or");
regex XOR("^xor");
regex TEST("^test");
regex SHL("^shl");
regex SHR("^shr");

// regexi za asemblerske naredbe sirine 3B ili 5B
regex CALL("^call");
regex JMP("^jmp");
regex JEQ("^jeq");
regex JNE("^jne");
regex JGT("^jgt");
regex PUSH("^push");
regex POP("^pop");
regex LDR("^ldr");
regex STR("^str");

// regexi za operand kojim se definise vrednost podatka
regex LITERAL("\\$((0x\\w+)|[0-9]+)");
regex SIM("\\$\\w+");
regex MEMLIT("((0x\\w+)|[0-9]+)");
regex REGDIR("((r[0-8])|pc|sp|psw)");
regex MEMSIM("\\w+");
regex MEMPCSIM("\\%\\w+");
regex MEMREG("\\[((r[0-8])|pc|sp|psw)\\]");                                 // [<reg>]
regex MEMREGLIT("\\[((r[0-8])|pc|sp|psw)\\+((0x\\w+)|[0-9]+)\\]");          // [<reg> + <literal>]
regex MEMREGSIM("\\[((r[0-8])|pc|sp|psw)\\+\\w+\\]");                       // [<reg> + <simbol> ]

// regexi za operand kojim se definise odrediste skoka
regex JMP_LITERAL("((0x\\w+)|[0-9]+)");
regex JMP_SIMBOL("\\w+");
regex JMP_PCSIM("\\%\\w+");
regex JMP_MEMLIT("\\*((0x\\w+)|[0-9]+)");
regex JMP_REGDIR("\\*((r[0-8])|pc|sp|psw)");
regex JMP_MEMSIM("\\*\\w+");
regex JMP_MEMREG("\\*\\[((r[0-8])|pc|sp|psw)\\]");                          // *[<reg>]
regex JMP_MEMREGLIT("\\*\\[((r[0-8])|pc|sp|psw)\\+((0x\\w+)|[0-9]+)\\]");      // *[<reg> + <literal>]
regex JMP_MEMREGSIM("\\*\\[((r[0-8])|pc|sp|psw)\\+\\w+\\]");

// regexi za linker
regex SIMBOL("\\.?\\w+[ \t]{2,}\\w+[ \t]{2,}\\w+[ \t]{2,}\\w+[ \t]{2,}\\w+[ \t]{2,}\\w+");
regex REL_ZAPIS("\\w+[ \t]{2,}\\w+[ \t]{2,}\\w+");                          // mora da se proveri nakon RelZapis_HEADER; posto je ovo opsiti slucaj
regex ZVEZDICA("\\*+");
regex RelZapis_SEKCIJA("\\#\\.rel\\.\\w+");
regex RelZapis_HEADER("Offset[ \t]{2,}Tip[ \t]{2,}Simbol");