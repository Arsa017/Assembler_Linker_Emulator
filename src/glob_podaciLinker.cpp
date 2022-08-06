#include "../inc/glob_podaciLinker.h"
#include <string>

int red_br_sekcijeL = 0;
string curr_sekcijaL = "";
int red_br_fajlaL = 0;

int sizeCnt = 0;
int redBrSekcijaFinal = 0;
int redBrFinal = 0;

string hex_kod = "";

ifstream ulazLinkera;       // input file stream  --> ulazni fajlovi linkera
ofstream izlaz;             // output file stream

list<SimbolLinker*> tabela_simbolaL;    
list<SimbolLinker*>::iterator iter_simbolL;
list<SimbolLinker*>::iterator pomIter_simbolL;
list<SimbolLinker*>::iterator iter_finalTable;

list<SimbolLinker*> final_tabela_simbolaL;

list<SimbolLinker*> lista_undSimbola;
list<SimbolLinker*>::iterator iter_undList;

list<RelZapisLinker*> lista_relZapisaL;
list<RelZapisLinker*>::iterator iter_relZapisL;

list<masKodLinker> srdZaIspis;
list<masKodLinker>::iterator iter_masinKodL;
list<masKodLinker>::iterator pomIterMK;


string decimal_u_oznaceni_hexaL(int broj) {      // -4   ==>   FF FF FF FC
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

string negativni_wordL(string s) {           // npr:  FF FF FF FC   ===>     FF FC
    string tmp = "";
    tmp += s[4];
    tmp += s[5];
    tmp += s[6];
    tmp += s[7];
    return tmp;
}

string hex_u_cetiri_bajta(string s) {
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

string little_endian_wordL(string s) {
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

int konverzija_hexa_u_intL(string hexaVrednost) {
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