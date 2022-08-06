#include "../inc/obrada_linije.h"
#include <cctype>
#include <cstdlib>
#include <cmath>

using namespace std;

void obrada_linije() {

    if (linija.size() == 0) {
        return;                     // prazan red
    }

    if (regex_search(linija, LABELA)) {
        obrada_labele(linija);                          // labela:      mov rax, a
        linija = regex_replace(linija, LABELA, "");     //      mov rax, a
        if(regex_search(linija, BELI_ZNAK)) {
            linija = regex_replace(linija, BELI_ZNAK, "");  // mov rax, a
        }
        if (linija.size() == 0) {
            // prazan red; u nastavku nema asemblerskih instrukcija
            return;
        }
        // postoji asemblerska instrukcijau liniji pa nju treba obraditi pre povratka u while petlju main-a
    }

    if (regex_search(linija, GLOBAL)) {
        linija = regex_replace(linija, GLOBAL, "");     //.global  a,    b,  c
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // a,b,c    <- f-ja uklanja sve razmake
        obrada_global(linija);
        return;
    }

    if (regex_search(linija, EXTERN)) {
        linija = regex_replace(linija, EXTERN, "");     //.extern a,    b, c
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // a,   b, c  => a,b,c
        obradi_extern(linija);
        return;
    }

    if (regex_search(linija, SECTION)) {    // obradjuje se sekcija; ne ispisuje nista u masinski kod
        obrada_sekcije(linija);
        return;
    }

    if (regex_search(linija, SKIP)) {               // .skip      9
        linija = regex_replace(linija, SKIP, "");   //          9
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // 9
        obrada_skip(linija);
        return;
    }

    if (regex_search(linija, WORD)) {           // linija = .word        2,  a, b
        linija = regex_replace(linija, WORD, "");   //   linija =        2,  a, b
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());      // linija = 2,a,b
        obrada_word();
        return;
    }

    // obrada instrukcije, tj. asemblerske naredbe
    obrada_asemblerske_naredbe();
}

void obrada_asemblerske_naredbe() {
    if (regex_search(linija, HALT) || regex_search(linija, IRET) || regex_search(linija, RET)) 
    {
        obrada_jednoBajtne_naredbe();
    }
    else if (regex_search(linija, INT) || regex_search(linija, XCHG) || regex_search(linija, ADD) || regex_search(linija, SUB) 
            || regex_search(linija, MUL) || regex_search(linija, DIV) || regex_search(linija, CMP) || regex_search(linija, NOT) || regex_search(linija, AND) 
            || regex_search(linija, OR) || regex_search(linija, XOR) || regex_search(linija, TEST) || regex_search(linija, SHL) || regex_search(linija, SHR)) 
    {
        obrada_dvoBajtne_naredbe();
    }
    else if (regex_search(linija, JMP) || regex_search(linija, JEQ) || regex_search(linija, JNE) || regex_search(linija, JGT) ||  
            regex_search(linija, PUSH) || regex_search(linija, POP) || regex_search(linija, LDR) || regex_search(linija, STR) || regex_search(linija, CALL))
    {
        obrada_naredbe_sirine_3_ili_5_bajta();
    } else {
        cout << "Greska na liniji: " << brojacLinije << endl;
        cout << "Nevalidna instrukcija: " << linija <<  endl;
        exit(1);
    }
}

void obrada_naredbe_sirine_3_ili_5_bajta() {

    if (regex_search(linija, PUSH)) {               //   push     r0
        linija = regex_replace(linija, PUSH, "");   //      r0
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end()); // r0  
        obrada_instrukcije_push();
    }

    if (regex_search(linija, POP)) {
        linija = regex_replace(linija, POP, "");
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());
        obrada_instrukcije_pop();
    }

    if (regex_search(linija, LDR)) {                            // ldr   r4,  operand
        linija = regex_replace(linija, LDR, "");                //    r4,  operand
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r4,operand
        obrada_instrukcije_ldr();
    }

    if (regex_search(linija, STR)) {                            // str  r4  , operand  
        linija = regex_replace(linija, STR, "");                //   r4  , operand 
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r4,operand
        obrada_instrukcije_str();
    }

    if (regex_search(linija, JMP)) {                            // jmp      operand
        linija = regex_replace(linija, JMP, "");                //      operand
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // operand
        obrada_instrukcije_jmp();
    }

    if (regex_search(linija, JEQ)) {                            // jeq      operand
        linija = regex_replace(linija, JEQ, "");                //       operand
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // operand
        obrada_instrukcije_jeq();
    }

    if (regex_search(linija, JNE)) {                            // jne      operand
        linija = regex_replace(linija, JNE, "");                //       operand
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // operand
        obrada_instrukcije_jne();
    }

    if (regex_search(linija, JGT)) {                            // jgt      operand
        linija = regex_replace(linija, JGT, "");                //      operand
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // operand
        obrada_instrukcije_jgt();
    }

    if (regex_search(linija, CALL)) {                           // call      operand
        linija = regex_replace(linija, CALL, "");               //      operand
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // operand
        obrada_instrukcije_call();
    }

}

void obrada_instrukcije_call() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "30";     // 1. bajt instrukcije <- OC instr. call - 0011 0000
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // visa 4 bita 2. bajta instrukcije; regD se ne koristi
    obrada_operanda_za_instrukciju_skoka(linija);
}

void obrada_instrukcije_jgt() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "53";     // 1. bajt instrukcije <- OC instr. jgt - 0101 0011
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // visa 4 bita 2. bajta instrukcije; regD se ne koristi
    obrada_operanda_za_instrukciju_skoka(linija);
}

void obrada_instrukcije_jne() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "52";     // 1. bajt instrukcije <- OC instr. jne - 0101 0010
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // visa 4 bita 2. bajta instrukcije; regD se ne koristi
    obrada_operanda_za_instrukciju_skoka(linija);
}

void obrada_instrukcije_jeq() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "51";     // 1. bajt instrukcije <- OC instr. jeq - 0101 0001
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // visa 4 bita 2. bajta instrukcije; regD se ne koristi
    obrada_operanda_za_instrukciju_skoka(linija);
}

void obrada_instrukcije_jmp() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "50";     // 1. bajt instrukcije <- OC instr. jmp - 0101 0000
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // visa 4 bita 2. bajta instrukcije; regD se ne koristi
    obrada_operanda_za_instrukciju_skoka(linija);
}

void obrada_operanda_za_instrukciju_skoka(string operand) {

    // operand = <literal>                  <------      neposredno adresiranje
    if (regex_match(operand, JMP_LITERAL)) {
        obrada_jmp_lit(operand);            
        return;                             
    }

    // operand = <simbol>                   <------      neposredno adresiranje
    if (regex_match(operand, JMP_SIMBOL)) {
        obrada_jmp_sim(operand);            
        return;
    }

    // operand = %<simbol>                  <------      PC-relativno adresiranje
    if (regex_match(operand, JMP_PCSIM)) {
        obrada_jmp_pcsim(operand);          
        return;
    }

    // operand = *<literal>                 <------      memorijsko direktno adresiranje
    if (regex_match(operand, JMP_MEMLIT)) {
        obrada_jmp_memlit(operand);
        return;
    }

    // operand = *<reg>                     <------      registarsko direktno adresiranje; obrada pre *<simbol>, posto je to opstiji slucaj
    if (regex_match(operand, JMP_REGDIR)) {
        obrada_jmp_regdir(operand);
        return;
    }

    // operand = *<simbol>                  <------      memorijsko direktno adresiranje
    if (regex_match(operand, JMP_MEMSIM)) {
        obrada_jmp_memsim(operand);
        return;
    }

    // operand = *[<reg>]                   <------      registarsko indirektno adresiranje
    if (regex_match(operand, JMP_MEMREG)) {
        obrada_jmp_memreg(operand);
        return;
    }

    // operand = *[<reg> + <literal>]       <------      registarsko indirektno sa pomerajem
    if (regex_match(operand, JMP_MEMREGLIT)) {
        obrada_jmp_memreglit(operand);
        return;
    }

    // operand = *[<reg> + <simbol>]        <------      registarsko indirektno sa pomerajem
    if (regex_match(operand, JMP_MEMREGSIM)) {
        obrada_jmp_memregsim(operand); 
        return;
    }

    // ako se nista ne prepozna prijavi gresku
    cout << "Greska na liniji: " << brojacLinije << endl;
    cout << "Nevalidan operand: " << operand <<  endl;
    exit(1);
}

// registarsko indirektno sa pomerajem
void obrada_jmp_memregsim(string opr) {                                 // opr = *[r4+nazivSimbola]
    string operand = regex_replace(opr, regex("^\\*\\["), "");          // operand = r4+nazivSimbola]
    operand = regex_replace(operand, regex("\\]$"), "");                // operand = r4+nazivSimbola

    vector<string> niz;         
    stringstream ss(operand);               // r4+nazivSimbola
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, '+');  
        niz.push_back(tmp);                 // niz = ["r4", "nazivSimbola"]
    }
    string registar = niz[0];               // registar = r4        
    string simbol = niz[1];                 // simbol   = nazivSimbola

    // obrada odabranog registra
    string srcReg;
    int broj_registra = vrati_broj_registra(registar);      // broj_registra = 4
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);                  // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;       // niza 4 bita 2. bajta instrukcije su odabrani registar
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "03";         // 3. bajt instrukcije ---> 0000 0011  # nema azuriranja; registarsko indirektno adresiranje sa pomerajem
    loc_counter += 3;                                               // lc sad ukazuje na adresu na koju treba smestiti operand; tj. prva 3 bajta instrukcije su generisana

    // obrada simbola
    bool postoji = false;
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == simbol) {
            postoji = true;
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    // lokalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->sekcija, red_br_sekcije, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                    lista_relZapisa.push_back(relokZapis);

                    string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);            // 4    ==>  0004
                    string data = little_endian_word(vrednost);                                     // 0004 ==>  0400
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;                         // simbol je lokalan pa ugradjujemo njegovu tekucu vrednost
                    loc_counter += 2;
                } else {
                    // globalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";   // 4. i 5. bajt inicijalizujemo nulama 
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";
                loc_counter += 2;
            }
        } 
    }

    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabeli simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(simbol, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // Kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";       // 4. i 5. bajt inicijalizujemo nulama
        loc_counter += 2;
    }

    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// registarsko indirektno sa pomerajem
void obrada_jmp_memreglit(string opr) {                                 // opr = *[r4+5]         ili opr = *[r4+0xff]
    string operand = regex_replace(opr, regex("^\\*\\["), "");          // operand = r4+5]
    operand = regex_replace(operand, regex("\\]$"), "");                // operand = r4+5

    vector<string> niz;         
    stringstream ss(operand);               // r0+5
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, '+');  
        niz.push_back(tmp);                 // niz = ["r0", "5"]
    }
    string registar = niz[0];               // registar = r0        
    string literal = niz[1];                // literal = 5    ||  literal = 0xff00

    // obrada odabranog registra
    string srcReg;
    int broj_registra = vrati_broj_registra(registar);      // broj_registra = 0
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);                  // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;               // niza 4 bita 2. bajta instrukcije su odabrani registar
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "03";                 // 3. bajt instrukcije ---> 0000 0011  # nema azuriranja; registarsko indirektno adresiranje sa pomerajem

    // obrada literala
    int broj = 0;       
    if (regex_search(literal, regex("^0x"))) {                  // literal = 0xf
        literal = regex_replace(literal, regex("^0x"), "");     // literal = f
        broj = konverzija_hexa_u_int(literal);                  // f -> 15
    } else {
        broj = stoi(literal);                                   // string -> int
    }

    string tmp = decimal_u_oznaceni_hexa(broj);                 // npr: 32 -> 20
    string data = little_endian_word(tmp);                      // 0020 -> 2000
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;     // 4. i 5. bajt instrukcije
    loc_counter +=5;                                            // instrukcije je sirine 5B

    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // razmak za testiranje
}

// registarsko indirektno adresiranje
void obrada_jmp_memreg(string opr) {                                // opr      = *[r4]
    string operand = regex_replace(opr, regex("^\\*\\["), "");      // oprerand = r4]
    operand = regex_replace(operand, regex("\\]$"), "");            // operand  = r4

    string srcReg;
    int broj_registra = vrati_broj_registra(operand);               // broj_registra = 4
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);                          // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;       // niza 4 bita 2. bajta instrukcije; SRC reg
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "02";         // 3. bajt instrukcije ---> 0000 0010  # nema azuriranja; registarsko indirektno
    loc_counter += 3;                                               // instrukcija je duzine 3 bajta
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " "; 
}

// memorijsko direktno adresiranje
void obrada_jmp_memsim(string opr) {                                    // opr = *nazivSimbola
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";              // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "04";             // 3. bajt instrukcije ---> 0000 0100 # nema azuriranja; memorijsko direktno adresiranje 
    loc_counter += 3;

    string operand = regex_replace(opr, regex("\\*"), "");              // operand = nazivSimbola

    bool postoji = false;
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == operand) {
            postoji = true;
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    // lokalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->sekcija, red_br_sekcije, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                    lista_relZapisa.push_back(relokZapis);
                    
                    string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);       // 4    ==>  0004
                    string data = little_endian_word(vrednost);                                // 0004 ==>  0400
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;                    // simbol je lokalan pa ugradjujemo njegovu tekucu vrednost
                    loc_counter += 2;
                } else {
                    // globalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";  // 4. i 5. bajt inicijalizujemo nulama 
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";      // 4. i 5. bajt inicijalizujemo nulama 
                loc_counter += 2;
            }
        }
    }

    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabeli simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(operand, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";   // 3. i 4. bajt inicijalizujemo nulama 
        loc_counter +=2;
    }

    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// registarsko direktno adresiranje
void obrada_jmp_regdir(string opr) {                            // opr    = *r4
    string operand = regex_replace(opr, regex("\\*"), "");      // oprand = r4

    string srcReg;
    int broj_registra = vrati_broj_registra(operand);           // broj_registra = 4
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);                      // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;           // niza 4 bita 2. bajta instrukcije
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "01";             // 3. bajt instrukcije  ---> 0000 0001   # nema azuriranja; registarsko direktno adresiranje 
    loc_counter += 3;
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " "; 
}

// memorijsko direktno adresiranje
void obrada_jmp_memlit(string opr) {                            // operand = *literal;  opr     = *5 ili opr     = *0xff00
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "04";     // 3. bajt instrukcije ---> 0000 0100 # nema azuriranja; memorijsko direktno adresiranje
                                                                
    string operand = regex_replace(opr, regex("\\*"), "");      // operand = 5  ili operand = 0xff00
    int broj = 0;
    if (regex_search(operand, regex("^0x"))) {                  // 0xf
        operand = regex_replace(operand, regex("^0x"), "");     // f
        broj = konverzija_hexa_u_int(operand);                  // f -> 15
    } else {
        broj = stoi(operand);                                   // string -> int
    }
    
    string tmp = decimal_u_oznaceni_hexa(broj);                 // 32   -> 20
    string data = little_endian_word(tmp);                      // 0020 -> 2000

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;
    loc_counter += 5;
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";
}

// PC-relativno adresiranje
void obrada_jmp_pcsim(string opr) {                             // operand = %simbol
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "7";      // niza 4 bita 2. bajta instrukcije; PC je odabrani registar
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "05";     // 3. bajt instrukcije ---> 0000 0101 # nema azuriranja; PC-relativno adresiranje
    loc_counter += 3;                                           // lc sad ukazuje na adresu na koju treba smestiti operand; tj. prva 3 bajta instrukcije su generisana

    string operand = regex_replace(opr, regex("\\%"), "");      // operand = nazivOpr

    bool postoji = false;
    int broj = 2;           // sirina adresnog polja
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == operand) {
            postoji = true;                                         // simbol postoji u tabelu simbola
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    //lokalan simbol
                    if ((*iter_simbol)->sekcija == red_br_sekcije) {
                        // lokalan simbol definisan u istoj sekciji kao instrukcija skoka; nema realokacioni zapis; razlika x-y je konstantna nezavisno od procesa linkovanja
                        // x - y = x - (* + 2)  ==> x - * - 2;      ----> x je vrednost simbola; * je adresno polje, odnosno loc_counter; a -2 je pomeraj od sledece instrukcije do adresnog polja
                        int offset = (*iter_simbol)->vrednost - loc_counter - 2;
                        string str;
                        bool negativan;
                        if (offset >= 0) {
                            negativan = false;          // pozitivan offset
                        } else {
                            negativan = true;           // negativan offset
                        }
                        // cout << "*** Negativan = " << negativan << endl;
                        // cout << (*iter_simbol)->vrednost << " " << loc_counter << " " << broj << endl;
                        // cout << offset << endl;
                        if (!negativan) {
                            str = decimal_u_oznaceni_hexa(offset);              // nema odsecanja posto je pozitivan broj
                        } else {
                            string tmp = decimal_u_oznaceni_hexa(offset);       // -4  ===> FF FF FF FC         negativan broj
                            str = negativni_word(tmp);                          // FF FF FF FC  ===>   FF FC 
                        }
                        
                        string le_str = little_endian_word(str);                // FF FC   ===>  FC FF;  0005 ====> 0500
                        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_str;
                        loc_counter += 2;
                    } else {
                        // lokalan simbol definisan u nekoj drugoj sekciji 
                        RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_PC16", (*iter_simbol)->sekcija, red_br_sekcije, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                        lista_relZapisa.push_back(relokZapis);
                        // asembler ugradjuje sve sto mu je poznato; tekucu vrednost simbola i pomeraj od sledece instrukcije do adresnog polja
                        // x = s(x) + d(x);  y = * + 2;     s(x) je adresa pocetka sekcije u kojoj je definisan simbol x, a d(x) je pomeraj do njega od pocetka sekcije
                        // x - y = s(x) + d(x) - * - 2;     ===>    [s(x) - *] + d(x) - 2;      linker razresava deo u zagradi, a ostalo asembler
                        int offset = (*iter_simbol)->vrednost - 2;
                        string str;
                        bool negativan;
                        if (offset >= 0) {
                            negativan = false;
                        } else {
                            negativan = true;
                        }
                        // cout << "*** Negativan = " << negativan << endl;
                        // cout << (*iter_simbol)->vrednost << " " << loc_counter << " " << broj << endl;
                        // cout << offset << endl;                        
                        if (!negativan) {
                            str = decimal_u_oznaceni_hexa(offset);              // nema odsecanja posto je pozitivan broj
                        } else {
                            string tmp = decimal_u_oznaceni_hexa(offset);       // -4  ===> FF FF FF FC         negativan broj
                            str = negativni_word(tmp);                          // FF FF FF FC  ===>   FF FC 
                        }

                        string le_str = little_endian_word(str);                // FF FC   ===>  FC FF;  0005 ====> 0500
                        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_str;
                        loc_counter += 2;
                    }
                } else {
                    // globalan simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_PC16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    // ugradjujemo -2 u masinski kod; sto nam je pomeraj od sledece instrukcije do adresnog polja; x - y = x - (* + 2) = [x - *] - 2
                    string tmp = decimal_u_oznaceni_hexa(-2);        // -2          ===> FF FF FF FE
                    string str = negativni_word(tmp);                // FF FF FF FE ===> FF FE
                    string le_str = little_endian_word(str);         // ff fe => fe ff
                    
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_str;
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 1, 2);    // operacija = 1;   informacija da je simbol upotrebljen u PC-rel. adresiranju
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";       // 4. i 5. bajt inicijalizujemo nulama
                loc_counter += 2;
            }
        }
    }
    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabeli simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(operand, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 1, 2);        // operacija = 1;   informacija da je simbol upotrebljen u PC-rel. adresiranju
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";           // 4. i 5. bajt inicijalizujemo nulama
        loc_counter +=2;
    }

    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// neposredno adresiranje
void obrada_jmp_sim(string opr) {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "00";     // 3. bajt instrukcije --> 0000 0000  #  nema azuriranja; neposredno adresiranje
    loc_counter += 3;                                           // lc sad ukazuje na adresu na koju treba smestiti operand; tj. prva 3 bajta instrukcije su generisana

    string operand = opr;
    bool postoji = false;
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == operand) {
            postoji = true;
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    // lokalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->sekcija, red_br_sekcije, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                    lista_relZapisa.push_back(relokZapis);
                    
                    string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);       // 4    ==>  0004
                    string data = little_endian_word(vrednost);                                // 0004 ==>  0400
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;                    // simbol je lokalan pa ugradjujemo njegovu tekucu vrednost
                    loc_counter += 2;
                } else {
                    // globalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";  // 4. i 5. bajt inicijalizujemo nulama 
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";      // 4. i 5. bajt inicijalizujemo nulama 
                loc_counter += 2;
            }
        }
    }

    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabeli simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(operand, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";   // 3. i 4. bajt inicijalizujemo nulama 
        loc_counter +=2;
    }

    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// neposredno adresiranje
void obrada_jmp_lit(string opr) {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";      // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "00";     // 3. bajt instrukcije --> 0000 0000  #  nema azuriranja; neposredno adresiranje

    string operand = opr;                                   // operand = 5 ili operand = 0xff00
    int broj = 0;
    if (regex_search(operand, regex("^0x"))) {              // 0xf
        operand = regex_replace(operand, regex("^0x"), ""); // f
        broj = konverzija_hexa_u_int(operand);              // f -> 15
    } else {
        broj = stoi(operand);                               
    }
    
    string tmp = decimal_u_oznaceni_hexa(broj);              // 32 -> 20
    string data = little_endian_word(tmp);                   // 20 00
    
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;        // 4. i 5. bajt instrukcije su za smestanje operanda
    loc_counter += 5;
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";
}

void obrada_instrukcije_str() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "b0";      // 1. bajt instrukcije <- OC instr. str - 1011 0000
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);   // niz = ["regD", "operand"];
    string regD = niz[0];
    string operand = niz[1];

    string destReg;
    int broj_registra = vrati_broj_registra(regD);
    if (broj_registra != -1) {
        destReg = to_string(broj_registra);                       // visa 4 bita 2. bajta instrukcije su dest. registar
    } else {
        cout << "Greska na liniji:" << brojacLinije << "\n" << "Instrukcija LDR; Nepostojeci registar: " << niz[0] << endl;
        exit(1);
    }
    
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += destReg;   // upis mas. kod za dest. registar u visa 4 bita 2. bajta instrukcije
    obrada_operanda_za_vrednost_podatka(operand);
}

void obrada_instrukcije_ldr() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "a0";      // 1. bajt instrukcije <- OC instr. ldr - 1010 0000
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);   // niz = ["regD", "operand"];
    string regD = niz[0];
    string operand = niz[1];

    string destReg;
    int broj_registra = vrati_broj_registra(regD);
    if (broj_registra != -1) {
        destReg = to_string(broj_registra);                       // visa 4 bita 2. bajta instrukcije su dest. registar
    } else {
        cout << "Greska na liniji:" << brojacLinije << "\n" << "Instrukcija LDR; Nepostojeci registar: " << niz[0] << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += destReg;    // upis mas. koda za dest. registar u visa 4 bita 2. bajta instrukcije
    obrada_operanda_za_vrednost_podatka(operand);                 
}



void obrada_operanda_za_vrednost_podatka(string operand) {

    // operand = $<literal>     <----- neposredno adresiranje
    if (regex_match(operand, LITERAL)) {
        obrada_literala(operand);      // operand = $5;   operand = $0xff00
        return;
    }

    // operand = $<simbol>                      <--- neposredno adresiranje
    if (regex_match(operand, SIM)) {
        obrada_simbola(operand);       // operand = $nazivSimbola
        return;
    }

    // operand = <literal>                      <--- memorijsko direktno adresiranje
    if (regex_match(operand, MEMLIT)) {
        obrada_memlit(operand);         // operand  = literal
        return;         
    }

    // operand = <reg>                          <--- registarsko direktno adresiranja; - mora pre <simbol> jer je <simbol> opstiji
    if (regex_match(operand, REGDIR)) {
        obrada_regdir(operand);
        return;
    }

    // operand = <simbol>                       <--- memorijsko direktno adresiranje
    if (regex_match(operand, MEMSIM)) {
        obrada_memsim(operand);
        return;
    }

    // operand = %simbol                        <--- vrednost iz memorije PC-rel adresiranjem
    if (regex_match(operand, MEMPCSIM)) {      
        obrada_mempcsim(operand);
        return;
    }

    // operand = [<reg>]                        <--- registarsko indirektno adresiranje
    if (regex_match(operand, MEMREG)) {
        obrada_memreg(operand);
        return;
    }

    // operand = [<reg> + <literal]             <--- registarsko indirektno sa pomerajem
    if (regex_match(operand, MEMREGLIT)) {
        obrada_memreglit(operand);
        return;
    }

    // operand = [<reg> + <simbol>]             <--- registarsko indirektno sa pomerajem
    if (regex_match(operand, MEMREGSIM)) {
        obrada_memregsim(operand);
        return;
    }

    // ako se nista ne prepozna prijavi gresku
    cout << "Greska na liniji: " << brojacLinije << endl;
    cout << "Nevalidan operand: " << operand <<  endl;
    exit(1);
}

//  vrednost iz memorije PC-rel adresiranjem
void obrada_mempcsim(string opr) {                                  // opr = %nazivOpr
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "7";          // niza 4 bita 2. bajta instrukcije; PC je odabrani registar
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "03";         // 3. bajt instrukcije --->  0000 0011  # nema azuriranja; neposredno adresiranje 
    loc_counter += 3;                                               // lc sad ukazuje na adresu na koju treba smestiti operand; tj. prva 3 bajta instrukcije su generisana

    string operand = regex_replace(opr, regex("\\%"), "");          // operand = nazivOpr

    bool postoji = false;
    int broj = 2;           // sirina adresnog polja
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == operand) {
            postoji = true;                                         // simbol postoji u tabelu simbola
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    //lokalan simbol
                    if ((*iter_simbol)->sekcija == red_br_sekcije) {
                        // lokalan simbol definisan u istoj sekciji kao instrukcija skoka; nema realokacioni zapis; razlika x-y je konstantna nezavisno od procesa linkovanja
                        // x - y = x - (* + 2)  ==> x - * - 2;      ----> x je vrednost simbola; * je adresno polje, odnosno loc_counter; a -2 je pomeraj od sledece instrukcije do adresnog polja
                        int offset = (*iter_simbol)->vrednost - loc_counter - 2;
                        string str;
                        bool negativan;
                        if (offset >= 0) {
                            negativan = false;          // pozitivan offset
                        } else {
                            negativan = true;           // negativan offset
                        }
                        // cout << "*** Negativan = " << negativan << endl;
                        // cout << (*iter_simbol)->vrednost << " " << loc_counter << " " << broj << endl;
                        // cout << offset << endl;
                        if (!negativan) {
                            str = decimal_u_oznaceni_hexa(offset);              // nema odsecanja posto je pozitivan broj
                        } else {
                            string tmp = decimal_u_oznaceni_hexa(offset);       // -4  ===> FF FF FF FC         negativan broj
                            str = negativni_word(tmp);                          // FF FF FF FC  ===>   FF FC 
                        }
                        
                        string le_str = little_endian_word(str);                // FF FC   ===>  FC FF;  0005 ====> 0500
                        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_str;
                        loc_counter += 2;
                    } else {
                        // lokalan simbol definisan u nekoj drugoj sekciji 
                        RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_PC16", (*iter_simbol)->sekcija, red_br_sekcije, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                        lista_relZapisa.push_back(relokZapis);
                        // asembler ugradjuje sve sto mu je poznato; tekucu vrednost simbola i pomeraj od sledece instrukcije do adresnog polja
                        // x = s(x) + d(x);  y = * + 2;     s(x) je adresa pocetka sekcije u kojoj je definisan simbol x, a d(x) je pomeraj do njega od pocetka sekcije
                        // x - y = s(x) + d(x) - * - 2;     ===>    [s(x) - *] + d(x) - 2;      linker razresava deo u zagradi, a ostalo asembler
                        int offset = (*iter_simbol)->vrednost - 2;
                        string str;
                        bool negativan;
                        if (offset >= 0) {
                            negativan = false;
                        } else {
                            negativan = true;
                        }
                        // cout << "*** Negativan = " << negativan << endl;
                        // cout << (*iter_simbol)->vrednost << " " << loc_counter << " " << broj << endl;
                        // cout << offset << endl;                        
                        if (!negativan) {
                            str = decimal_u_oznaceni_hexa(offset);              // nema odsecanja posto je pozitivan broj
                        } else {
                            string tmp = decimal_u_oznaceni_hexa(offset);       // -4  ===> FF FF FF FC         negativan broj
                            str = negativni_word(tmp);                          // FF FF FF FC  ===>   FF FC 
                        }

                        string le_str = little_endian_word(str);                // FF FC   ===>  FC FF;  0005 ====> 0500
                        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_str;
                        loc_counter += 2;
                    }
                } else {
                    // globalan simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_PC16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    // ugradjujemo -2 u masinski kod; sto nam je pomeraj od sledece instrukcije do adresnog polja
                    string tmp = decimal_u_oznaceni_hexa(-2);        // -2          ===> FF FF FF FE
                    string str = negativni_word(tmp);                // FF FF FF FE ===> FF FE
                    string le_str = little_endian_word(str);         // ff fe => fe ff
                    
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_str;
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 1, 2);    // operacija = 1;   informacija da je simbol upotrebljen u PC-rel. adresiranju
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";       // 4. i 5. bajt inicijalizujemo nulama
                loc_counter += 2;
            }
        }
    }
    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabeli simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(operand, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 1, 2);        // operacija = 1;   informacija da je simbol upotrebljen u PC-rel. adresiranju
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";           // 4. i 5. bajt inicijalizujemo nulama
        loc_counter +=2;
    }
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// registarsko indirektno sa pomerajem
void obrada_memregsim(string opr) {                                 // opr = [r4+nazivSimbola]        
    string operand = regex_replace(opr, regex("^\\["), "");         // operand = r4+nazivSimbola]
    operand = regex_replace(operand, regex("\\]$"), "");            // operand = r4+nazivSimbola

    vector<string> niz;         
    stringstream ss(operand);               // r4+nazivSimbola
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, '+');  
        niz.push_back(tmp);                 // niz = ["r4", "nazivSimbola"]
    }
    string registar = niz[0];               // registar = r4        
    string simbol = niz[1];                 // simbol   = nazivSimbola

    // obrada odabranog registra
    string srcReg;
    int broj_registra = vrati_broj_registra(registar);      // broj_registra = 4
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);                  // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;       // niza 4 bita 2. bajta instrukcije su odabrani registar
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "03";         // 3. bajt instrukcije ---> 0000 0011  # nema azuriranja; registarsko indirektno adresiranje sa pomerajem
    loc_counter += 3;                                               // lc sad ukazuje na adresu na koju treba smestiti operand; tj. prva 3 bajta instrukcije su generisana

    // obrada simbola
    bool postoji = false;
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == simbol) {
            postoji = true;
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    // lokalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->sekcija, red_br_sekcije, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                    lista_relZapisa.push_back(relokZapis);

                    string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);            // 4    ==>  0004
                    string data = little_endian_word(vrednost);                                     // 0004 ==>  0400
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;                         // simbol je lokalan pa ugradjujemo njegovu tekucu vrednost
                    loc_counter += 2;
                } else {
                    // globalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";   // 4. i 5. bajt inicijalizujemo nulama 
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";
                loc_counter += 2;
            }
        } 
    }

    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabeli simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(simbol, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // Kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";       // 4. i 5. bajt inicijalizujemo nulama
        loc_counter += 2;
    }

    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// registarsko indirektno sa pomerajem
void obrada_memreglit(string opr) {                                 // opr = [r4+5]         ili opr = [r4+0xff]
    string operand = regex_replace(opr, regex("^\\["), "");         // operand = r4+5]
    operand = regex_replace(operand, regex("\\]$"), "");            // operand = r4+5

    vector<string> niz;         
    stringstream ss(operand);               // r0+5
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, '+');  
        niz.push_back(tmp);                 // niz = ["r0", "5"]
    }
    string registar = niz[0];               // registar = r0        
    string literal = niz[1];                // literal = 5    ||  literal = 0xff00

    // obrada odabranog registra
    string srcReg;
    int broj_registra = vrati_broj_registra(registar);      // broj_registra = 0
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);                  // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;               // niza 4 bita 2. bajta instrukcije su odabrani registar
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "03";                 // 3. bajt instrukcije ---> 0000 0011  # nema azuriranja; registarsko indirektno adresiranje sa pomerajem
    // obrada literala
    int broj = 0;   
    
    if (regex_search(literal, regex("^0x"))) {                  // literal = 0xf
        literal = regex_replace(literal, regex("^0x"), "");     // literal = f
        broj = konverzija_hexa_u_int(literal);                  // f -> 15
    } else {
        broj = stoi(literal);
    }

    string tmp = decimal_u_oznaceni_hexa(broj);                 // npr: 32 -> 20
    string data = little_endian_word(tmp);                      // 0020 -> 2000
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;     // 4. i 5. bajt instrukcije
    loc_counter +=5;                                            // instrukcije je sirine 5B
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // razmak za testiranje
}

// registarsko indirektno adresiranje
void obrada_memreg(string opr) {                                    // opr     = [r4]
    string operand = regex_replace(opr, regex("^\\["), "");         // operand = r4]
    operand = regex_replace(operand, regex("\\]$"), "");                // operand = r4

    string srcReg;
    int broj_registra = vrati_broj_registra(operand);               // broj_registra = 4
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);                          // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }
    
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;       // niza 4 bita 2. bajta instrukcije
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "02";         // 3. bajt instrukcije ---> 0000 0010  # nema azuriranja; registarsko indirektno
    loc_counter += 3;                                               // instrukcija je duzine 3 bajta
    
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";        // dodato za testiranje
}

// memorijsko direktno adresiranje
void obrada_memsim(string opr) {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";        // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi    
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "04";       // 3. bajt instrukcije --> 0000 0100   # nema azuriranja; memorijsko direktno adresiranje
    loc_counter += 3;                                             // lc sad ukazuje na adresu na koju treba smestiti operand; tj. prva 3 bajta instrukcije su generisana

    string operand = opr;       // ime simbola
    bool postoji = false;
    for(iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == operand) {
            postoji = true;
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    // lokalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->sekcija, red_br_sekcije, 'l');   // rel. zapis se pravi u odnosu na sekciju u kojoj je simbol DEFINISAN
                    lista_relZapisa.push_back(relokZapis);

                    string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);        // 4    ==> 0004
                    string data = little_endian_word(vrednost);                                 // 0004 ==> 0400
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;                     // simbol je lokalan pa ugradjujemo njegovu tekucu vrednost u adresno polje
                    loc_counter += 2;
                } else {
                    // globalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";   // 4. i 5. bajt inicijalizujemo nulama
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";       // 4. i 5. bajt incijalizujemo nulama
                loc_counter += 2;
            }
        }
    }

    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabelu simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(operand, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";       // 4. i 5. bajt inicijalizujemo nulama
        loc_counter += 2;
    }
//    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// registarsko direktno adresiranja
void obrada_regdir(string opr) {                        // opr = r4 
    string srcReg;
    int broj_registra = vrati_broj_registra(opr);       // broj_registra = 4
    if (broj_registra != -1) {
        srcReg = to_string(broj_registra);              // 4; int -> string
    } else {
        cout << "Greska na liniji: " << brojacLinije << "\n" << "Nepostojeci odabrani(src) registar: "  << opr << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += srcReg;           // niza 4 bita 2. bajta instrukcije
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "01";             // 3. bajt instrukcije  ---> 0000 0001   # nema azuriranja; registarsko direktno adresiranje
    loc_counter += 3;                                                   // instrukcija je duzine 3 bajta
//    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";
}


// memorijsko direktno adresiranje
void obrada_memlit(string opr) {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";          // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "04";         // 3. bajt instrukcije ---> 0000 0100  # nema azuriranja; memorijsko adresiranje 

    string operand = opr;       // operand = 5 ili operand = 0xff00
    int broj = 0;
    if (regex_search(operand, regex("^0x"))) {                  // 0xf
        operand = regex_replace(operand, regex("^0x"), "");     // f
        broj = konverzija_hexa_u_int(operand);                  // f -> 15
    } else {
        broj = stoi(operand);
    }

    string tmp = decimal_u_oznaceni_hexa(broj);         // 32   -> 20
    string data = little_endian_word(tmp);              // 0020 -> 2000;    0005 -> 0500

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;         // 4. i 5 bajt instrukcije
    // sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";
    loc_counter += 5;
}

// neposredno adresiranje
void obrada_simbola(string opr) {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";        // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "00";       // 3. bajt instrukcije --> 0000 0000  #  nema azuriranja; neposredno adresiranje
    loc_counter += 3;                                             // lc sad ukazuje na adresu na koju treba smestiti operand; tj. prva 3 bajta instrukcije su generisana

    string operand = regex_replace(opr, regex("\\$"), "");        // operand = nazivOpr         

    bool postoji = false;
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == operand) {
            postoji = true;
            if ((*iter_simbol)->definisan) {
                // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                if ((*iter_simbol)->vidljivost == 'l') {
                    // lokalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->sekcija, red_br_sekcije, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                    lista_relZapisa.push_back(relokZapis);
                    
                    string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);       // 4    ==>  0004
                    string data = little_endian_word(vrednost);                                // 0004 ==>  0400
                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;                    // simbol je lokalan pa ugradjujemo njegovu tekucu vrednost
                    loc_counter += 2;
                } else {
                    // globalni simbol
                    RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g'); // rel. zapis se pravi u odnosu na simbol
                    lista_relZapisa.push_back(relokZapis);

                    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";  // 4. i 5. bajt inicijalizujemo nulama 
                    loc_counter += 2;
                }
            } else {
                // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                (*iter_simbol)->dodaj(zapis);
                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";      // 4. i 5. bajt inicijalizujemo nulama 
                loc_counter += 2;
            }
        }
    }

    if (!postoji) {
        // slucaj kada 1. put nailazimo na upotrebu simbola
        // dodajemo simbol u tabeli simbola;
        red_br_simbola++;
        Simbol* sym = new Simbol(operand, 0, 0, 'l', red_br_simbola, 0, false, false);
        lista_simbola.push_back(sym);

        // kreira se zapis o obracanju unapred
        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
        sym->dodaj(zapis);

        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";   // 3. i 4. bajt inicijalizujemo nulama 
        loc_counter +=2;
    }

//    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";      // dodato za testiranje
}

// neposredno adresiranje
void obrada_literala(string opr) {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "f";        // niza 4 bita 2. bajta instrukcije; SRC registar se ne koristi
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "00";       // 3. bajt instrukcije --> 0000 0000  #  nema azuriranja; neposredno adresiranje
    
                                                            // operand = $5 ili operand = $0xff00
    string operand = regex_replace(opr, regex("\\$"), "");  // operand = 5  ili operand = 0xff00
    int broj = 0;
    if (regex_search(operand, regex("^0x"))) {              // 0xf
        operand = regex_replace(operand, regex("^0x"), ""); // f
        broj = konverzija_hexa_u_int(operand);              // f -> 15
    } else {
        broj = stoi(operand);                               
    }
    
    string tmp = decimal_u_oznaceni_hexa(broj);              // 32 -> 20
    string data = little_endian_word(tmp);                   // 20 00
    
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += data;        // 4. i 5. bajt instrukcije su za smestanje operanda
//    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += " ";
    loc_counter += 5;
}

// pop regD    # instrukciju realizujemo pomocu instrukcije za ucitavanje podataka -> LDR (podatak se ucitava sa steka)
void obrada_instrukcije_pop() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "a0";                 // mas. kod LDR instrukcije  <--- 1. bajt instrukcije
    int broj_registra = vrati_broj_registra(linija);                        // r0 ---> 0
    string destReg;
    if (broj_registra != -1) {
        destReg += to_string(broj_registra);                                // visa 4 bita 2. bajta su dest. registar
        destReg += "6";                                                     // niza 4 bita 2. bajta su SP registar, tj. odabrani registar
    } else {
        cout << "Greska na liniji:" << brojacLinije << "\n" << "Instrukcija POP; Nepostojeci registar: " << linija << endl;
        exit(1);
    }
    // 2. bajt instrukcije
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += destReg;
    // 3. bajt instrukcije      *0100 0010*     ---->   regD <= mem16[sp];  sp <= sp+2;   
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "42";     // prvo skidamo sadrzaj sa steka i ucitavamo u destinacioni registar, pa uvecavamo sp za 2
    loc_counter += 3;
}

// push regD   # instrukciju realizujemo pomocu instrukcije za smestanje podataka -> STR (ovde se podatak smesta na stek)
void obrada_instrukcije_push() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "b0";                 // mas. kod STR instrukcije  <--- 1. bajt instrukcije
    int broj_registra = vrati_broj_registra(linija);                        // r0 ---> 0
    string destReg;
    if (broj_registra != -1) {
        destReg = to_string(broj_registra);                                 // visa 4 bita 2. bajta su dest. registar
        destReg += "6";                                                     // niza 4 bita 2. bajta su SP registar, tj. odabrani registar
    } else {
        cout << "Greska na liniji:" << brojacLinije << "\n" << "Instrukcija PUSH; Nepostojeci registar: " << linija << endl;
        exit(1);
    }
    // 2. bajt instrukcije
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += destReg;             
    // 3. bajt instrukcije   *0001 0010*   ---->   nacin azuriranja sp registra i nacin adresiranja (sp <= sp-2; mem16[sp] <= regD;)
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "12";     // registar je sp, a prvo smanjujemo za dva - pravimo prostor na steku
    loc_counter += 3;
}

void obrada_dvoBajtne_naredbe() {
    if (regex_search(linija, INT)) {                //int       r0      # parsiranje
        linija = regex_replace(linija, INT, "");    //        r0
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  //r0
        obradi_instrukciju_int();
    }

    if (regex_search(linija, XCHG)) {                       // xchg     r0,         r1
        linija = regex_replace(linija, XCHG, "");           //       r0,       r1
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r0,r1
        obradi_instrukciju_xchg();
    }

    if (regex_search(linija, ADD)) {                        // add    r0,   r1
        linija = regex_replace(linija, ADD, "");            //     r0,   r1
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end()); // r0,r1
        obradi_instrukciju_add();
    }

    if (regex_search(linija, SUB)) {                         // sub         r0      ,    r1
        linija = regex_replace(linija, SUB, "");             //         r0   ,     r1  
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r0,r1
        obradi_instrukciju_sub();
    }

    if (regex_search(linija, MUL)) {                         // mul    r0,  r1
        linija = regex_replace(linija, MUL, "");             //     r0,  r1
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end()); // r0,r1
        obradi_instrukciju_mul();
    }

    if (regex_search(linija, DIV)) {                        // div   r4,   r5
        linija = regex_replace(linija, DIV, "");            //    r4,    r5
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r4,r5
        obradi_instrukciju_div();
    }

    if (regex_search(linija, CMP)) {                        // cmp     r4  ,   r1
        linija = regex_replace(linija, CMP, "");            //      r4  ,   r1
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r4,r5
        obradi_instrukciju_cmp();
    }

    if (regex_search(linija, NOT)) {                        // not    r1
        linija = regex_replace(linija, NOT, "");            //     r1
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r1
        obradi_instrukciju_not();
    }

    if (regex_search(linija, AND)) {                        // and    r1    ,   r2
        linija = regex_replace(linija, AND, "");            //     r1    ,   r2
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());  // r1,r2
        obradi_instrukciju_and();
    }

    if (regex_search(linija, OR)) {
        linija = regex_replace(linija, OR, "");
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());
        obradi_instrukciju_or();
    }

    if (regex_search(linija, XOR)) {
        linija = regex_replace(linija, XOR, "");
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());
        obradi_instrukciju_xor();
    }

    if (regex_search(linija, TEST)) {
        linija = regex_replace(linija, TEST, "");
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());
        obradi_instrukciju_test();
    }

    if (regex_search(linija, SHL)) {
        linija = regex_replace(linija, SHL, "");
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());
        obradi_instrukciju_shl();
    }

    if (regex_search(linija, SHR)) {
        linija = regex_replace(linija, SHR, "");
        linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());
        obradi_instrukciju_shr();
    }
}

void obradi_instrukciju_shr() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "91";
    string tmp = odredi_DestSrc_bajt(niz, "SHR");
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_shl() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "90";                 // mas. kod SHL instrukcije - 1001 0000
    string tmp = odredi_DestSrc_bajt(niz, "SHL");
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_test() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "84";                  // mas. kod TEST instrukcije - 1000 0100
    string tmp = odredi_DestSrc_bajt(niz, "TEST");
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_xor() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "83";                 // mas. kod AND instrukcije - 1000 0011
    string tmp = odredi_DestSrc_bajt(niz, "XOR");
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_or() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "82";                // mas. kod OR instrukcije - 1000 0010
    string tmp = odredi_DestSrc_bajt(niz, "OR");
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_and() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "81";                  // mas. kod AND instrukcije - 1000 0001
    string tmp = odredi_DestSrc_bajt(niz, "AND");                            // 2. bajt instrukcije AND
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;                   
    loc_counter += 2;
}

void obradi_instrukciju_cmp() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "74";                   // mas. kod CMP instrukcije - 0111 0100
    string tmp = odredi_DestSrc_bajt(niz, "CMP");                            // 2. bajt instrukcije CMP
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_div() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "73";                   // mas. kod DIV instrukcije - 0111 0011
    string tmp = odredi_DestSrc_bajt(niz, "DIV");                             // 2. bajt instrukcije DIV
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_mul() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "72";                    // mas. kod MUL instrukcije - 0111 0010
    string tmp = odredi_DestSrc_bajt(niz, "MUL");                              // 2. bajt instrukcije MUL
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_sub() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "71";                    // mas. kod SUB instrukcije - 0111 0001 
    string tmp = odredi_DestSrc_bajt(niz, "SUB");                              // 2. bajt instrukcije SUB
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_add() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "70";                    // mas. kod ADD instrukcije - 0111 0000 
    string tmp = odredi_DestSrc_bajt(niz, "ADD");                              // 2. bajt instrukcije ADD   
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_xchg() {
    vector<string> niz = vrati_niz_Registra_Iz_Linije(linija);         
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "60";                     // mas. kod XCHG instrukcije  
    string tmp = odredi_DestSrc_bajt(niz, "XCHG");                              // 2. bajt instrukcije XCHG
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += tmp;
    loc_counter += 2;
}

void obradi_instrukciju_not() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "80";                 // mas. kod NOT instrukcije - 1000 0000

    int broj_registra = vrati_broj_registra(linija);    // r0 ---> 0
    string destReg;
    if (broj_registra != -1) {
        destReg = to_string(broj_registra);
        destReg += "f ";                                // niza 4 bita koja referenciraju src registar proveriti !!!
    } else {
        cout << "Greska na liniji:" << brojacLinije << "\n" << "Instrukcija NOT; Nepostojeci registar: " << linija << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += destReg;
    loc_counter += 2;
}

void obradi_instrukciju_int() {
    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "10";                 // mas. kod INT instrukcije
       
    // provera o kom se registru radi
    int broj_registra = vrati_broj_registra(linija);    // r0 ---> 0
    string destReg;
    if (broj_registra != -1) {
        destReg = to_string(broj_registra);
        destReg += "f";
    } else {
        cout << "Greska na liniji:" << brojacLinije << "\n" << "Instrukcija INT; Nepostojeci registar: " << linija << endl;
        exit(1);
    }

    sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += destReg;
    loc_counter +=2;
}

void obrada_jednoBajtne_naredbe() {
    // ugradjujemo masinski kod instrukcije za tekucu sekciju u zavisnosti od mnemonika instrukcije
    if (regex_search(linija, HALT)) {
        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "00";         // mas. kod HALT instrukcije je 00
    } else if (regex_search(linija, IRET)) {
        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "20";         // mas. kod IRET instrukcije je 20
    } else {
        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "40";         // mas. kod RET  instruckije je 40
    }

    loc_counter += 1;    // obradjene naredbe su sirine 1B pa uvecavamo LC za 1;
}

void obrada_word() {
    vector<string> niz; 
    stringstream ss(linija);
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, ',');
        niz.push_back(tmp);
    }
    

    // niz = ["2", "a", "b"]
    for (unsigned int i = 0; i < niz.size(); i++) {
        bool negativan = false;     // pp: broj nije negativan
        bool isNumber = true;       // pp: inicijalno jeste broj
        bool obradjen = false;      // nije obradjen; fleg za optimizaciju pozitivnih i negativnih brojeva
        int broj;                   
        if (regex_search(niz[i], regex("^\\-"))) {    // -12
            niz[i] = regex_replace(niz[i], regex("^\\-"), "");   // 12
            for(unsigned int j = 0; j < niz[i].size(); j++) {
                if (!isdigit(niz[i][j])) {                    
                    cout << "Greska na liniji:" << brojacLinije << "\n" << "Greska! Neregularan negativan broj!" << endl;
                    exit(1);
                }
            }
            negativan = true;                           
            int tmp = stoi(niz[i]);
            broj = 0 - tmp;                             // imamo string konvertovan u negativan broj
            isNumber = true;
            obradjen = true;
        }

        if (!obradjen) {                                // da li je pozitivan broj; 12
            for(unsigned int j = 0; j < niz[i].size(); j++) {
                if (!isdigit(niz[i][j])) {                    
                    isNumber = false;                   // ako ima karakter koji nije cifra, onda nije LITERAL vec SIMBOL
                }
            }
            if (isNumber) {
                negativan = false;
                broj = stoi(niz[i]);                    // imamo string konvertovan u pozitivan broj
                isNumber = true;
                obradjen = true;
            }           
        }

        if (isNumber) {
        // obrada literala cija je decimalna vrednost smestena u broj; WORD inicijalizujemo vrednoscu literala
            string str = ""; 
            if (!negativan) {   
                str = decimal_u_oznaceni_hexa(broj);            // nista ce ne odseca;          pozitivan broj
            } else {    
                string tmp = decimal_u_oznaceni_hexa(broj);     // -4  ===> FF FF FF FC         negativan broj
                str = negativni_word(tmp);                      // FF FF FF FC  ===>   FF FC 
            }

            string le_str = little_endian_word(str);            //  FF FC => FC FF;    0004 => 04 00
            sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_str;
            loc_counter += 2;
        } else {
        // obrada simbola   ....
    
            bool postoji = false;
            string currSymName = niz[i];
            for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
                if ((*iter_simbol)->naziv == currSymName) {
                    postoji = true;
                    if ((*iter_simbol)->definisan) {
                        // simbol postoji u tabeli simbola i DEFINISAN JE -> kao 2. prolaz asemblera
                        if ((*iter_simbol)->vidljivost == 'l') {
                            // lokalan simbol
                            RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->sekcija, red_br_sekcije, 'l');  // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                            lista_relZapisa.push_back(relokZapis);

                            string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);    // 4   ===>  0004
                            string le_vrednost = little_endian_word(vrednost);                      // 0004 ==>  0400
                            sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += le_vrednost;
                            loc_counter += 2;
                        } else {
                            // globalan simbol
                            RelZapis* relokZapis = new RelZapis(loc_counter, "R_386_16", (*iter_simbol)->redniBroj, red_br_sekcije, 'g');           // rel. zapis se pravi u odnosu na simbol 
                            lista_relZapisa.push_back(relokZapis);

                            sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";
                            loc_counter += 2;
                        }
                    } else {
                        // simbol postoji u tabeli simbola ali NIJE definisan ===> kreira se zapis o obracanju unapred
                        Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                        (*iter_simbol)->dodaj(zapis);
                        sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";
                        loc_counter += 2;
                    }
                }
            }

            if (!postoji) {
                // slucaj kada 1. put nailazimo na upotrebu simbola
                // dodajemo simbol u tabeli simbola;
                red_br_simbola++;
                Simbol* sym = new Simbol(currSymName, 0, 0, 'l', red_br_simbola, 0, false, false);
                lista_simbola.push_back(sym);

                // kreira se zapis o obracanju unapred
                Flink* zapis = new Flink(loc_counter, red_br_sekcije, 0, 2);
                sym->dodaj(zapis);

                sadrzajZaIspis[red_br_sekcije - 1].masinskiKod += "0000";   // lokaciju inicijalizujemo nulama
                loc_counter += 2;                                           // uvecavanje LC-a za velicnu obradjene direktive
            }

        }
    }
}

void obrada_labele(string linija) {
    string nazivLabele = "";
    for(unsigned int i = 0; i < linija.size(); i++) {
        if (linija[i] != ':') {
            nazivLabele += linija[i];
        } else {
            break;
        }
    }
    // provera da li labala postoji u listi labela; ako se nalazi proveravamo da li je definisana, ako jeste prijavljujemo gresku, 
    // a ako nije dodefinisemo je ako je simbol vec u tabeli simbola tu je zato sto se nasao u nekoj instrukciji kao operand 
    // ili zato sto je bio deklarisan direktivom .global/.extern
    bool postoji = false;
    for(iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        if ((*iter_simbol)->naziv == nazivLabele) {
            // da li je eksterni simbol
            if((*iter_simbol)->extrn) {
                cout << "Greska na liniji:" << brojacLinije << "\n" << "Eksterni simbol: " + nazivLabele + " ima definiciju!" << endl;
                exit(1);
            } else {
                // simbol nije eksterni pa ga dodefinisemo
                if ((*iter_simbol)->definisan) {
                    cout << "Greska na liniji:" << brojacLinije << "\n" << "Dvostruka definicija simbola: " + nazivLabele << endl;
                    exit(1); 
                } else {
                    postoji = true;
                    (*iter_simbol)->sekcija = red_br_sekcije;
                    (*iter_simbol)->vrednost = loc_counter;
                    (*iter_simbol)->definisan = true;
                break;
                }
            }
        }
    }
    if (!postoji) {
        // labela se nije nalazila u listi labela; dodajemo je u nju
        red_br_simbola++;
        Simbol* sym = new Simbol(nazivLabele, red_br_sekcije, loc_counter, 'l', red_br_simbola, 0, true, false);
        lista_simbola.push_back(sym);
    }

    /*
    string reg = labela + ":";
    regex l(reg);
    linija = regex_replace(linija, l, ""); 
    */
}

void obrada_skip(string linija) {   // 9
    int broj = 0;
    broj = stoi(linija);        // konverzija stringa u int vrednost
    if (curr_sekcija != "und") {
        string pom = "";
        for(int i = 0; i < broj; i++)
            pom += "00";                // alokacija prostora cija je velicina jednaka broju bajtova specificiranim dohvacenim brojem
        sadrzajZaIspis[red_br_sekcije-1].masinskiKod += pom;
    }
    loc_counter += broj;    // uvecavamo location_counter za velicinu generisanog sadrzaja
}

void obrada_end(string linija) {
    if (curr_sekcija != "und") {
        for (iter_simbol = lista_sekcija.begin(); iter_simbol != lista_sekcija.end(); iter_simbol++) {
            if ((*iter_simbol)->naziv == curr_sekcija) {
                (*iter_simbol)->velicina = loc_counter;
            }
        }
    }
}

void obradi_extern(string linija) {  // linija = "a,b,c"
    vector<string> niz;
    stringstream ss(linija);
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, ',');      // , je delimiter
        niz.push_back(tmp);         // niz = ["a", "b", "c"]
    }

    for(unsigned int i = 0; i < niz.size(); i++) {
        bool pronadjen = false;
        for(iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
            if ((*iter_simbol)->naziv == niz[i]) {
                pronadjen = true;
                if ((*iter_simbol)->definisan == false) {
                    (*iter_simbol)->vidljivost = 'g';
                    (*iter_simbol)->extrn = true;
                    break;
                } else {
                    cout << "Greska na liniji:" << brojacLinije << "\n" << "Definisan simbol: " << (*iter_simbol)->naziv << " ne moze da bude externi!" << endl;
                    exit(1);
                }
            }
        }
        // ako simbol nije pronadjen dodajemo ga u tabelu simbola i oznacavamo da je Globalan
        if(!pronadjen) {
            red_br_simbola++;
            Simbol* sym = new Simbol(niz[i], 0, 0, 'g', red_br_simbola, 0, false, true);
            lista_simbola.push_back(sym);
        }
    }
}


void obrada_global(string linija) {  // linija = "a,b,c"
    vector<string> niz;
    stringstream ss(linija);
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, ',');      // , je delimiter
        niz.push_back(tmp);         // niz = ["a", "b", "c"]
    }
    // za svaki simbol provera dal postoji u tabeli simbola i azuriranje fleg-a za vidljivost

    for (unsigned int i = 0; i < niz.size(); i++) {
        bool pronadjen = false;
        for(iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
            if ((*iter_simbol)->naziv == niz[i]) {
                pronadjen = true;
                if ((*iter_simbol)->definisan == false) {
                    // pronadjeni simbol nije definisan, pa azuriramo fleg na globalan
                    (*iter_simbol)->vidljivost = 'g';
                    (*iter_simbol)->extrn = false;
                    break;
                } else {
                    cout << "Greska na liniji:" << brojacLinije << "\n" << "Simbol: " << (*iter_simbol)->naziv << " je vec definisan kao lokalan!" << endl;
                    exit(1);
                }
            }
        }
        // ako simbol nije pronadjen dodajemo ga u tabelu simbola i oznacavamo da je Globalan

        if (!pronadjen) {
            red_br_simbola++;
            Simbol* simbol = new Simbol(niz[i], 0, 0, 'g', red_br_simbola, 0, false, false);   // inicijalna sekcija za koju vezujemo simbol je 0, posto jos uvek nije definisan
            lista_simbola.push_back(simbol); 
        }
    }
}

void obrada_sekcije(string str) {
    regex tmpRegex("\\.section\\s*");    // .section         mojaSekcija     # brisemo bele znakove do moje sekcije
    linija = regex_replace(str, tmpRegex, "");  // mojaSekcija
    string sekcija = "";
    for (unsigned int i = 0; i < linija.length(); i++) {
        if (isalpha(linija[i]) || ((char)linija[i] == 46)) {        // ascii kod . je 46
            sekcija += linija[i];
        } else {
            break;
        }
    }

    // proveravamo da li je nailazak na definiciju nove sekcije; tada se prethodna sekcija ovde zavrsava, pa azuriramo njenu velicinu
    if (curr_sekcija != "und") {
        for (iter_simbol = lista_sekcija.begin(); iter_simbol != lista_sekcija.end(); iter_simbol++) {
            if ((*iter_simbol)->naziv == curr_sekcija) {
                (*iter_simbol)->velicina = loc_counter;
            }
        }
    }

    loc_counter = 0;
    red_br_sekcije++;
    Simbol* simbol = new Simbol(sekcija, red_br_sekcije, 0, 'l', red_br_sekcije, 0, true, false);  // kreiranje objekta koji reprezentuje novu sekciju
    lista_sekcija.push_back(simbol);    // dodavanje nove sekcije u listu svih sekcija

    curr_sekcija = sekcija;
    struct masinKod tmp;        // kreiranje strukture u kojoj cemo cuvati generisani masinski kod za posmatranu sekciju
    tmp.nazivSekcije = curr_sekcija;
    tmp.masinskiKod = "";
    sadrzajZaIspis.push_back(tmp); //index u vectoru je red_br_sekcije-1(jer je index od 0)
}

