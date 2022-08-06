#include "../inc/instrukcije.h"
#include <string>
#include <bitset>
#include <sstream>

using namespace std;


void interpretiranjeInstrukcija() {
    // cout << "Interpretiranje instrukcija!" << endl;

    while(!izvrsen_halt) {
        // cout << "*** PC: " << r[7] << endl;
        int index = 2 * r[7];
        string op_code = "";
        op_code += memorija[index++];
        op_code += memorija[index];

        // cout << "op_code: " << op_code << endl;


        if (op_code == HALT) {
            obradi_halt();
            continue;
        }
        if (op_code == IRET) {
            obradi_iret();
            continue;
        }
        if (op_code == RET) {
            obradi_ret();
            continue;
        }

        if (op_code == INT) {
            obradi_int();
            continue;
        }

        if (op_code == XCHG) {
            obradi_xchg();
            continue;
        }

        if (op_code == ADD) {
            obradi_add();
            continue;
        }

        if (op_code == SUB) {
            obradi_sub();
            continue;
        }

        if (op_code == MUL) {
            obradi_mul();
            continue;
        }

        if (op_code == DIV) {
            obradi_div();
            continue;
        }

        if (op_code == NOT) {
            obradi_not();
            continue;
        }

        if (op_code == AND) {
            obradi_and();
            continue;
        }

        if (op_code == OR) {
            obradi_or();
            continue;
        }

        if (op_code == XOR) {
            obradi_xor();
            continue;
        }

        if (op_code == TEST) {
            obradi_test();
            continue;
        }

        if (op_code == SHR) {
            obradi_shr();
            continue;
        }

        if (op_code == SHL) {
            obradi_shl();
            continue;
        }

        if (op_code == CMP) {
            obradi_cmp();
            continue;
        }

        if (op_code == LDR) {
            obradi_ldr();
            continue;
        }

        if (op_code == STR) {
            obradi_str();
            continue;
        }

        if (op_code == JMP) {
            obradi_jmp();
            continue;
        }

        if (op_code == JEQ) {
            obradi_jeq();
            continue;
        }

        if (op_code == JNE) {
            obradi_jne();
            continue;
        }

        if (op_code == JGT) {
            obradi_jgt();
            continue;
        }

        if (op_code == CALL) {
            obradi_call();
            continue;
        }

        cout << "Greska! Nepostojeci operacioni kod: " << op_code << endl;          // ako nije prepoznata nijedna instrukcija; generise se zahtev za prekid; ulaz broj 1 IV tabele
        r[7] = 2;                                               // ulaz 1 iv tabele za obradu gresaka
    }

    ispis_stanja_procesora();
}

void obradi_jmp() {
    // cout << "------------------" << endl;
    // cout << "Instrukcija JMP" << endl;

    // r[3] = 30;
    // cout << "r3: " << r[3] << endl; 
    short operand = operand_instrukcije_skoka();
    // cout << "operand: " << operand << endl;
    r[7] = operand;


}

void obradi_jeq() {
    short operand = operand_instrukcije_skoka();
    // r[8] = 0x0001;
    if (r[8] & FLAG_Z)                                  // uslov za jeq:  Z = 1
        r[7] = operand;
}

void obradi_jne() {
    short operand = operand_instrukcije_skoka();
    //  r[8] = 0x0001;
    if (!(r[8] & FLAG_Z))                               // uslov za jeq:  Z = 0
        r[7] = operand;
}

void obradi_jgt() {
    short operand = operand_instrukcije_skoka();        // uslov za jgt:  Z == 0 and  N == O

    // r[8] = 1;
    short Z = r[8] & FLAG_Z;
    short O = (r[8] & FLAG_O) >> 1;
    short N = (r[8] & FLAG_N) >> 3;

    if (Z == 0 && N == O) 
        r[7] = operand; 
}

void obradi_call() {
    // push(r[7]);
    // cout <<"PC pre ulaska u f-ju: " << r[7] << endl;
    short operand = operand_instrukcije_skoka();  
    // cout <<"PC za stek f-ju: " << r[7] << endl;
    push(r[7]);
    r[7] = operand;      
    // cout <<"PC nakon skoka: " << r[7] << endl;
}

int operand_instrukcije_skoka() {
    // 2. bajt instrukcije
    r[7] += 1;
    int index = 2 * r[7];
    string tRegD = ""; string tRegS = "";
    tRegD = memorija[index++]; tRegS = memorija[index];

    int regD, regS;
    regD = -1;                                              // destinacioni registar se ne koristi u instrukciji skoka -> tRegD = ffff
    if (tRegS != "f") {
        regS = broj_registra(tRegS);
    } else {
        regS = -1;
    }

    // 3. bajt instrukcije
    r[7] += 1;
    index = 2 * r[7];
    string upd = "", adr = "";
    upd += memorija[index++];
    adr += memorija[index];
    int update, adresiranje;
    update = broj_registra(upd);                            // samo iskoriscene pomocne f-je
    adresiranje = broj_registra(adr);

    int operand = dohvati_operand(regD, regS, update, adresiranje);

    return operand;
}

void obradi_str() {
    // cout << "------------------" << endl;
    // cout << "Instrukcija STR" << endl;

    // 2. bajt instrukcije
    r[7] += 1;
    int index = 2 * r[7];

    string tRegD = ""; string tRegS = "";
    tRegD = memorija[index++]; tRegS = memorija[index];

    int regD, regS;
    regD = broj_registra(tRegD);
    if (tRegS != "f") {
        regS = broj_registra(tRegS);
    } else {
        regS = -1;
    }

    // 3. bajt instrukcije
    r[7] += 1;
    index = 2 * r[7];
    string upd = "", adr = "";
    upd += memorija[index++];
    adr += memorija[index];
    int update, adresiranje;
    update = broj_registra(upd);                            // samo iskoriscene pomocne f-je
    adresiranje = broj_registra(adr);


    // za instrukciju store nije dozvoljeno neposredno adresiranje
    if (adresiranje == IMMED) {
        cout << "Neispravan nacin adresiranja(IMMED) za instrukciju STR!" << endl;
        r[7] = 2;                   // ulaz 1 prekidne rutine; zaustavljanje procesora
        return;
    }

    if (adresiranje == REGDIR) {
        r[regS] = r[regD];
        r[7] += 1;
    }

    // za instrukciju store nije dozvoljeno PC-rel adresiranje
    if (adresiranje == PCREL) {
        cout << "Neispravan nacin adresiranja(PC-rel) za instrukciju STR!" << endl;
        r[7] = 2;                   // ulaz 1 prekidne rutine; zaustavljanje procesora
        return;
    }

    if (adresiranje == REGIND) {
        //  cout << "b0_SP -> pre smestanja na stek: " << r[6] << endl;
        if (update == 1) r[regS] -= 2;
        if (update == 2) r[regS] += 2;
       

        int tmpAdr = r[regS];                                   // operand u memoriji na adresi ukazanoj vrednoscu u odabranom registru
        int tmpIndex = 2 * tmpAdr;                              // na ovoj adresi smestamo vrednost r[regD]

        if (update == 3) r[regS] -= 2;
        if (update == 4) r[regS] += 2;

        // ako je regS = 6; onda se radi o instrukciji PUSH
        if (regS == 6) {
            smestanje_na_stek(tmpIndex, r[regD]);
        } else {
            smestanje_u_memoriju(tmpIndex, r[regD]); 
            // cout << "b0_upisana vednost u memoriju: " << memorija[tmpIndex+0] << memorija[tmpIndex+1] << memorija[tmpIndex+2] << memorija[tmpIndex+3] << endl; 
            // cout << "b0_SP -> nakon smestanja u memoriju: " << r[6] << endl;
        }

        //  smestanje_u_memoriju(tmpIndex, r[regD]);         
        r[7] += 1;        
    }

    if (adresiranje == REGINDPOM) {
        if (update == 1) r[regS] -= 2;
        if (update == 2) r[regS] += 2;

        // formiranje adrese operanda
        r[7] += 1;                                              // pc ukazuje na 4. bajt intrukcije
        int index = 2 * r[7];
        int payload = dohvati_staru_vrednost(index);            // operand = mem[r[regS] + payload]
        int addrOpr = r[regS] + payload;                        // izracunavanje adrese operanda; adresa je zbir vrednosti odabranog registra i oznacenog pomeraja
        index = 2 * addrOpr;
        
        if (update == 3) r[regS] -= 2;
        if (update == 4) r[regS] += 2;

        smestanje_u_memoriju(index, r[regD]);
        r[7] += 2;
        // cout << "upisana vednost: " << memorija[index+0] << memorija[index+1] << memorija[index+2] << memorija[index+3] << endl; 
    }

    if (adresiranje == MEM) {
        // cout << "str MEM" << endl;
        r[7] += 1;                                              // pc ukazuje na 4. bajt intrukcije
        int index = 2 * r[7];
        int payload = dohvati_staru_vrednost(index);            // operand = mem[payload]                           
        index = 2 * payload;                                    // operand je u memoriji na adresi ukazanoj payload-om

        // cout << "pre upisa vednosti: " << memorija[index+0] << memorija[index+1] << memorija[index+2] << memorija[index+3] << endl; 

        smestanje_u_memoriju(index, r[regD]);
        r[7] += 2;
        // cout << "posle upisa vednosti: " << memorija[index+0] << memorija[index+1] << memorija[index+2] << memorija[index+3] << endl; 
    }

}

void obradi_ldr(){
    // cout << "------------------" << endl;
    // cout << "Instrukcija ldr" << endl;
    r[7] += 1;
    int index = 2 * r[7];
    string tRegD = ""; string tRegS = "";
    tRegD = memorija[index++]; tRegS = memorija[index];

    int regD, regS;
    regD = broj_registra(tRegD);
    if (tRegS != "f") {
        regS = broj_registra(tRegS);
    } else {
        regS = -1;
    }

    // 3. bajt instrukcije
    r[7] += 1;
    index = 2 * r[7];
    string upd = "", adr = "";
    upd += memorija[index++];
    adr += memorija[index];
    int update, adresiranje;
    update = broj_registra(upd);                            // samo iskoriscene pomocne f-je
    adresiranje = broj_registra(adr);

    int operand = dohvati_operand(regD, regS, update, adresiranje);

    r[regD] = operand;
}

int dohvati_operand(int regD, int regS, int update, int adresiranje) {
    // cout << "registri: " << regD << " " << regS << endl;
    // cout << "update: " << update << " adresiranje: " << adresiranje << endl;
    int operand = 0;
    
    if (adresiranje == IMMED) {
       r[7] += 1;                                               // pc ukazuje na 4. bajt intrukcije
       int index = 2 * r[7];
       operand = dohvati_staru_vrednost(index);
       r[7] += 2;                                               // pc ce pokazivati na sledecu instrukciju
    }

    if (adresiranje == REGDIR) {
        operand = r[regS];
        r[7] += 1;                                              // pc ce pokazivati na sledecu instrukciju
    }

    if (adresiranje == PCREL) {
        r[7] += 1;                                               // pc ukazuje na 4. bajt intrukcije
        int index = 2 * r[7];
        operand = dohvati_staru_vrednost(index);
        operand += (r[regS] + 2);                                // +2 je zbog skaliranja; *PC ukazuje na 1. sledecu instrukciju, a trenutno posmatrana je velicine 5B
        r[7] += 2;                                               // pc ce pokazivati na sledecu instrukciju
    }

    if (adresiranje == REGIND) {
        // cout << "** REGIND **" << endl;
        // cout << "POP -> pre smestanja sp: " << r[6] << endl;
        if (update == 1) r[regS] -= 2;
        if (update == 2) r[regS] += 2;

        int tmpAdr = r[regS];                                   // operand u memoriji na adresi ukazanoj vrednoscu u odabranom registru
        int tmpIndex = 2 * tmpAdr;
    
        // cout << "r[regS]: " << r[regS] << endl;

        if (update == 3) r[regS] -= 2;
        if (update == 4) r[regS] += 2;

        // ako je regS == 6; radi se o instrukciji POP
        if (regS == 6) {
            operand = skidanje_sa_steka(tmpIndex);
        } else {
            operand = dohvati_staru_vrednost(tmpIndex);
        }

        // operand = dohvati_staru_vrednost(tmpIndex);
        // operand  = tmpAdr;
        r[7] += 1;                                              // pc ce pokazivati na sledecu instrukciju
        // cout << "POP -> nakon smestanja sp: " << r[6] << endl;
    }

    if (adresiranje == REGINDPOM) {
        if (update == 1) r[regS] -= 2;
        if (update == 2) r[regS] += 2;

        // formiranje adrese operanda
        r[7] += 1;                                              // pc ukazuje na 4. bajt intrukcije
        int index = 2 * r[7];
        int payload = dohvati_staru_vrednost(index);            // operand = mem[r[regS] + payload]
        int addrOpr = r[regS] + payload;                        // izracunavanje adrese operanda; adresa je zbir vrednosti odabranog registra i oznacenog pomeraja
        // cout << "adresa operanda: " << addrOpr << endl;
        index = 2 * addrOpr;
        
        if (update == 3) r[regS] -= 2;
        if (update == 4) r[regS] += 2;

        // ako je regS == 6; radi se o instrukciji POP
        if (regS == 6) {
            operand = skidanje_sa_steka(index);
        } else {
            operand = dohvati_staru_vrednost(index);
        }

        // operand = dohvati_staru_vrednost(index);
        r[7] += 2;
    }

    if (adresiranje == MEM) {
        // cout << "*** Ulazi  ***" << endl;
        r[7] += 1;                                              // pc ukazuje na 4. bajt intrukcije
        int index = 2 * r[7];
        int payload = dohvati_staru_vrednost(index);            // operand = mem[payload]                              
        index = 2 * payload;                                    // operand je u memoriji na adresi ukazanoj payload-om

        operand = dohvati_staru_vrednost(index);
        r[7] += 2;
    }

    return operand;
}

void obradi_test() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    setovanje_flegova(TEST, r[regD], r[regS]);
    r[7] += 1;
}

void obradi_shr() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    setovanje_flegova(SHR, r[regD], r[regS]);
    r[regD] = r[regD] >> r[regS]; 
    r[7] += 1;
}

void obradi_shl() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);
    
    setovanje_flegova(SHL, r[regD], r[regS]);
    r[regD] = r[regD] << r[regS]; 
    r[7] += 1;
}

void obradi_cmp() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);
    
    setovanje_flegova(CMP, r[regD], r[regS]);
    r[7] += 1;
}


void setovanje_flegova(string oc, short value1, short value2) {
    
    if (oc == TEST) {
        short result = value1 - value2;
        r[8] = result == 0 ? r[8] | FLAG_Z : r[8] & ~FLAG_Z;
        r[8] = result < 0  ? r[8] | FLAG_N : r[8] & ~FLAG_N;
    }

    if (oc == SHR) {
        short result = value1 >> value2;
        r[8] = result == 0 ? r[8] | FLAG_Z : r[8] & ~FLAG_Z;
        r[8] = result < 0  ? r[8] | FLAG_N : r[8] & ~FLAG_N;

        if (value2 == 1) {
            if (value1 & 0x0001) r[8] |= FLAG_C;
            else r[8] &= ~FLAG_C;
        } else {
            if (((value1 >> (value2-1))) & 0x0001) r[8] |= FLAG_C;
            else r[8] &= ~FLAG_C;
        }
    }

    if (oc == SHL) {
        short result = value1 << value2;
        r[8] = result == 0 ? r[8] | FLAG_Z : r[8] & ~FLAG_Z;
        r[8] = result < 0  ? r[8] | FLAG_N : r[8] & ~FLAG_N;

        short carry = 0;
        for (int i = 0; i < value2; i++) {
            carry = value1 & 0x8000;
            value1 = value1 << 1;
        }
        if (carry == 0) {
            r[8] &= ~FLAG_C;
        } else {
            r[8] |= FLAG_C;
        }
    }

    if (oc == CMP) {
        // cout << "vrednost reg: " << r[0] << " " << r[1] << endl;
        short result = value1 - value2;
        r[8] = result == 0 ? r[8] | FLAG_Z : r[8] & ~FLAG_Z;
        r[8] = result < 0  ? r[8] | FLAG_N : r[8] & ~FLAG_N;

        if (value1 < value2) {
            r[8] |= FLAG_C;
        } else {
            r[8] &= ~FLAG_C;
        }

        if (result >= -32768 || result <= 32767 ) {         // opseg je od -2^(16-1) do 2^(16-1) - 1    
            r[8] &= ~FLAG_O;                                // nema prekoracenja
        } else {
             r[8] |= FLAG_O;                                // ima prekoracenja
        }
    }
    
}

void obradi_xor() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    r[regD] = r[regD] ^ r[regS];
    r[7] += 1;
}

void obradi_or() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    r[regD] = r[regD] | r[regS];
    r[7] += 1;
}

void obradi_and() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    r[regD] = r[regD] & r[regS];
    r[7] += 1;
}

void obradi_not() {
    r[7] += 1;
    int index = 2 * r[7];
    string tRegD = ""; string tRegS = "";
    tRegD = memorija[index++]; tRegS = memorija[index];

    int regD = broj_registra(tRegD);

    r[regD] = ~r[regD];
    r[7] += 1;
}

void obradi_div() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    r[regD] = r[regD] / r[regS];
    r[7] += 1;
}

void obradi_mul() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    r[regD] = r[regD] * r[regS];
    r[7] += 1;
}

void obradi_sub() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    r[regD] = r[regD] - r[regS];
    r[7] += 1;
}

void obradi_add() {
    int regD, regS;
    dest_src_regIndex(&regD, &regS);

    // cout << "PRE sabiranja: " << r[regD] << " " << r[regS] << endl;

    r[regD] = r[regD] + r[regS];

    // cout << "NAKON sabiranja: " << r[regD] << " " << r[regS] << endl;
    r[7] += 1;
}

void obradi_xchg() {
   int regD, regS;
    dest_src_regIndex(&regD, &regS);
           
    short tmp = r[regD];
    r[regD] = r[regS];
    r[regS] = tmp;
    r[7] += 1;
}

void obradi_int() {
    r[7] += 1;
    int index = 2 * r[7];
    string tmpReg = "";
    tmpReg += memorija[index];
    int regD = broj_registra(tmpReg);

    push(r[7]+1);                               // <--- da li smesta na stek staru vrednost PC-a
    push(r[8]);                                 // push psw
    // cout <<"PSW za SKOK u PREKIDNE RUTINE: " << r[8] << endl;
    // cout <<"PC za SKOK u PREKIDNE RUTINE: " << r[7]+1 << endl;

    r[7] = (r[regD] % 8) * 2;                   // racunanje pc-ja
    // cout << "novi pc: " <<  r[7] << endl;
    int tmp = r[7] * 2;
    r[7] = dohvati_staru_vrednost(tmp);
}


void obradi_halt() {
    izvrsen_halt = true;
}

void obradi_ret() {
    r[7] = pop();
    // cout <<"PC za povratak u glavni program: " << r[7] << endl;
}

void obradi_iret() {
    r[8] = pop();
    // cout <<"PSW za povratak iz PREKIDNE RUTINE: " << r[7] << endl;
    r[7] = pop();
    // cout <<"PC za povratak iz PREKIDNE RUTINE: " << r[7] << endl;
}

void push(short val) {
    // cout << "PUSH -> pre smestanja sp: " << r[6] << endl;
    r[6] = r[6] - 2;
    int index = 2 * r[6];                                   // sp = 4096
    smestanje_na_stek(index, val);
    // cout << "PUSH -> posle smestanja sp: " << r[6] << endl;
}

short pop() {              
    // cout << "POP -> pre skidanja sp: " << r[6] << endl;                          
    unsigned int index = (unsigned short) r[6] * 2; 
    short rezultat = skidanje_sa_steka(index);
    r[6] = r[6] + 2;
    // cout << "POP -> posle skidanja sp: " << r[6] << endl;
    return rezultat;
}



short skidanje_sa_steka(int index) {
    string str = "";
    str += memorija[index-1];
    str += memorija[index-0];
    str += memorija[index-3];
    str += memorija[index-2];

    // cout << "*** Skidamo sa steka: " << str << endl;

    short nadjenaVrednost;
    if (str[0] == 'f'){
        // negativan broj
        string negativanBroj = "ffff";
        negativanBroj += str[0];
        negativanBroj += str[1];
        negativanBroj += str[2];
        negativanBroj += str[3];

        unsigned int x;
        std::stringstream ss;
        ss << std::hex << negativanBroj;
        ss >> x;

        nadjenaVrednost = x;
    } else {
        // pozitivan broj
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> x;
        nadjenaVrednost = x;
    }
    
    return nadjenaVrednost;
}


void smestanje_na_stek(int index, short val) {
    string str;
    bool negativan;
    if (val >= 0) {
        negativan = false;
    } else {
        negativan = true;
    }

    if (!negativan) {
        str = decimal_u_oznaceni_hexaE(val);               // nema odsecanja posto je pozitivan broj
    } else {
        string tmp = decimal_u_oznaceni_hexaE(val);        // -4  ===> FF FF FF FC         negativan broj
        str = negativni_wordE(tmp);                             // FF FF FF FC  ===>   FF FC
    }
                                                            // str = 00 0a
    string le_str = little_endian_wordE(str);              //  str = 0a 00
    memorija[index--] = le_str[3];
    memorija[index--] = le_str[2];
    memorija[index--] = le_str[1];
    memorija[index--] = le_str[0];
    // cout << "*** Stavljamo na stek: " << le_str << endl;
}

void ispis_stanja_procesora() {
    int i = 0;
    while(i++ < 48) cout << "-";
    cout << endl;

    cout << "Emulated proccesor executed halt instruction" << endl;

    std::bitset<16> psw(r[8]);
    cout << "Emulated proccesor state: psw=0b" << psw << endl;
    
    string reg0 = "", reg1 = "", reg2 = "", reg3 = "", reg4 = "", reg5 = "", reg6 = "", reg7 = "";
    reg0 = vrednost_u_registru_za_prikaz(r[0]);
    reg1 = vrednost_u_registru_za_prikaz(r[1]);
    reg2 = vrednost_u_registru_za_prikaz(r[2]);
    reg3 = vrednost_u_registru_za_prikaz(r[3]);
    reg4 = vrednost_u_registru_za_prikaz(r[4]);
    reg5 = vrednost_u_registru_za_prikaz(r[5]);
    reg6 = vrednost_u_registru_za_prikaz(r[6]);
    reg7 = vrednost_u_registru_za_prikaz(r[7]);

    cout << "r0=0x" << reg0 << "    " << "r1=0x" << reg1 << "    " << "r2=0x" << reg2 << "    " << "r3=0x" << reg3 << endl;
    cout << "r4=0x" << reg4 << "    " << "r5=0x" << reg5 << "    " << "r6=0x" << reg6 << "    " << "r7=0x" << reg7 << endl;

    // cout<< endl<< endl;

    // for (int i =0; i < 9; i++) {
    //     cout << "r" << i << ": " << r[i] << endl;
    // }
}