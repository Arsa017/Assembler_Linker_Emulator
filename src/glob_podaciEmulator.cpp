#include "../inc/glob_podaciEmulator.h"
#include <string>

ifstream ulazEmulatora;

string hex_kod = "";

char memorija[131072];

unsigned short r[9];

short pc = 0;           
short sp = 0;
short psw = 0;

bool izvrsen_halt = false;

int START_STACK = 8192;         // stek pocinje na adresi 0x2000

short FLAG_Z = 0x0001;
short FLAG_O = 0x0002;
short FLAG_C = 0x0004;
short FLAG_N = 0x0008;


// OC-kod za asemblerske naredbe sirine 1B
string HALT = "00";
string IRET = "20";
string RET  = "40"; 

// OC-kod za asemblerske naredbe sirine 2B
string INT  = "10";
string XCHG = "60";
string ADD  = "70";
string SUB  = "71";
string MUL  = "72";
string DIV  = "73";
string CMP  = "74";
string NOT  = "80";
string AND  = "81";
string OR   = "82";
string XOR  = "83";
string TEST = "84";
string SHL  = "90";
string SHR  = "91";

// OC-kod za asemblerske naredbe sirine 3B ili 5B
string CALL = "30";
string JMP  = "50";
string JEQ  = "51";
string JNE  = "52";
string JGT  = "53";
// extern string PUSH;
// extern string POP;
string LDR  = "a0";
string STR  = "b0";

int IMMED = 0;
int REGDIR = 1;
int PCREL = 5;
int REGIND = 2;
int REGINDPOM = 3;
int MEM = 4;


string decimal_u_oznaceni_hexaE(int broj) {      // -4   ==>   FF FF FF FC
    map<int, char> m;
    char cifra = '0';
    char c = 'a';
    
    for (int i = 0; i < 16; i++) {
        if (i < 10) 
            m[i] = cifra++;
        else 
            m[i] = c++;
    }

    string rezultat = "";
    if (broj == 0) {
        return "0";
    }
    if (broj > 0) {
        while(broj) {
            rezultat = m[broj % 16] + rezultat;
            broj /= 16;
        }
    } else {
        u_int n = broj;
        while (n) {
            rezultat = m[n % 16] + rezultat;
            n /= 16;
        }
    }

    return rezultat;                        // predstava u 4B  --->   FF FF FF FC
}

string negativni_wordE(string s) {           // npr:  FF FF FF FC   ===>     FF FC
    string tmp = "";
    tmp += s[4];
    tmp += s[5];
    tmp += s[6];
    tmp += s[7];
    return tmp;
}

string hex_u_cetiri_bajtaE(string s) {
    string tmp = "";            // cuva odgovarajuci string
    string pom = "";
    switch (s.size())
    {
    case 4:                     // s   = 1234
        tmp = s;                // tmp = 1234
        break;
    case 3:                     // s   = 123
        pom = "0";              // pom = 0123
        pom += s;               // tmp = 0123
        tmp = pom;  
        break;
    case 2:                     // s   = 12
        pom = "00";             // pom = 00
        pom += s;               // pom = 0012
        tmp = pom;
        break;
    case 1:
        pom = "000";
        pom += s;
        tmp = pom;
        break;
    default:
        break;
    }

    string res = "";
    res += tmp[0];
    res += tmp[1];
    res += tmp[2];
    res += tmp[3];

    return res;
}

string little_endian_wordE(string s) {
    string tmp = "";            // cuva odgovarajuci string
    string pom = "";
    switch (s.size())
    {
    case 4:                     // s   = 1234
        tmp = s;                // tmp = 1234
        break;
    case 3:                     // s   = 123
        pom = "0";              // pom = 0123
        pom += s;               // tmp = 0123
        tmp = pom;  
        break;
    case 2:                     // s   = 12
        pom = "00";             // pom = 00
        pom += s;               // pom = 0012
        tmp = pom;
        break;
    case 1:
        pom = "000";
        pom += s;
        tmp = pom;
        break;
    default:
        break;
    }

    string res = "";            // 00 04 ===> 04 00 
    res += tmp[2];
    res += tmp[3];
    res += tmp[0];
    res += tmp[1];

    return res;
}

int konverzija_hexa_u_intE(string hexaVrednost) {
    int duzina = hexaVrednost.size();
    int osnova = 1;
    int vrednost = 0;

    for (int i = duzina - 1; i >= 0; i--)
    {
        if(hexaVrednost[i] >= '0' && hexaVrednost[i] <= '9') {
            vrednost += (hexaVrednost[i] - 48) * osnova;
            osnova = osnova * 16;
        } else {
            if (hexaVrednost[i] >= 'A' && hexaVrednost[i] <= 'F') {
                vrednost += (hexaVrednost[i] - 55) * osnova;
                osnova = osnova * 16;
            } else {
                if (hexaVrednost[i] >= 'a' && hexaVrednost[i] <= 'f') {
                    vrednost += (hexaVrednost[i] - 87) * osnova;
                    osnova = osnova * 16;
                }
            }
        }
    }
    return vrednost;
}

int broj_registra(string s) {
    if (s == "0") return 0;
    if (s == "1") return 1;
    if (s == "2") return 2;
    if (s == "3") return 3;
    if (s == "4") return 4;
    if (s == "5") return 5;
    if (s == "6") return 6;
    if (s == "7") return 7;
    if (s == "8") return 8;
    
    return -1;
}

int dohvati_staru_vrednost(int indeks) {
    int tmpIndeks1 = indeks;         
    int nadjenaVrednost;
                                                // na poziciji index => FE FF
    char c0 = memorija[tmpIndeks1++];            // c0 = F
    char c1 = memorija[tmpIndeks1++];            // c1 = E
    char c2 = memorija[tmpIndeks1++];            // c2 = F
    char c3 = memorija[tmpIndeks1++];            // c3 = F

    string value = "";
    value += c2; 
    value += c3; 
    value += c0;
    value += c1;

    // if (c2 == 'f') {
    //     // negativan broj
    //     string negativanBroj = "ffff";          // F000 -> 1000
    //     negativanBroj += c2;
    //     negativanBroj += c3;
    //     negativanBroj += c0;
    //     negativanBroj += c1;

    //     unsigned int x;
    //     std::stringstream ss;
    //     ss << std::hex << negativanBroj;
    //     ss >> x;

    //     nadjenaVrednost = x;

    //     cout << "nadjena vrednost: " << nadjenaVrednost << "    x: " << x << endl;       <----- ispis za proveru
    // } else {
        // pozitivan broj
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << value;
        ss >> x;
        nadjenaVrednost = x;

        // cout << "nadjena vrednost: " << nadjenaVrednost << "    x: " << x << endl;       <----- ispis za proveru
    // }

    return nadjenaVrednost;
}

void dest_src_regIndex(int* dest, int* src) {
    r[7] += 1;
    int index = 2 * r[7];
    string tRegD = ""; string tRegS = "";
    tRegD = memorija[index++]; tRegS = memorija[index];

    int regD = broj_registra(tRegD);
    int regS = broj_registra(tRegS);

    *dest = regD;
    *src = regS;

    // cout << "** Pom f-ja **" << endl;
    // cout << "regD: " << *dest << "src: " << regS << endl; 
}

void smestanje_u_memoriju(short index, short val) {
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
    memorija[index++] = le_str[0];
    memorija[index++] = le_str[1];
    memorija[index++] = le_str[2];
    memorija[index++] = le_str[3];
}

string vrednost_u_registru_za_prikaz(int val) {
    int offset = val;
    string str;
    bool negativan;
    if (offset >= 0) {
        negativan = false;
    } else {
        negativan = true;
    }
                                							
    if (!negativan) {
        str = decimal_u_oznaceni_hexaE(offset);              // nema odsecanja posto je pozitivan broj
        str = hex_u_cetiri_bajtaE(str);
    } else {
        string tmp = decimal_u_oznaceni_hexaE(offset);       // -4  ===> FF FF FF FC         negativan broj
        str = negativni_wordE(tmp);                          // FF FF FF FC  ===>   FF FC 
    }

    return str;
}