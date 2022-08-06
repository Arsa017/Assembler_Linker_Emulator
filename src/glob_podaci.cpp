#include "../inc/glob_podaci.h"


ifstream ulaz;              // input file stream
ofstream izlaz;             // output file stream

// vector<ifstream> ulazi;     // ulazni fajlovi za linker

int brojacLinije = 1;

string linija = "";
string curr_sekcija = "und";

int loc_counter = 0;
int red_br_sekcije = 0;
int red_br_simbola = 0;

list<Simbol*> tabela_simbola;    // na kraju prolaza prvo smestimo sve sekcije, a zatim sve simbole ciji je redBr uvecan za broj sekcija, kako bi se ocuvao redosled
list<Simbol*> lista_sekcija;
list<Simbol*> lista_simbola;
list<Simbol*>::iterator iter_simbol;

list<RelZapis*> lista_relZapisa;
list<RelZapis*>::iterator iter_relZapis;

list<Flink*>::iterator iter_flink;

vector<masinKod> sadrzajZaIspis;
vector<masinKod>::iterator iter_masinKod;


string decimal_u_oznaceni_hexa(int broj) {      // -4   ==>   FF FF FF FC
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

string negativni_word(string s) {           // npr:  FF FF FF FC   ===>     FF FC
    string tmp = "";
    tmp += s[4];
    tmp += s[5];
    tmp += s[6];
    tmp += s[7];
    return tmp;
}

string little_endian_word(string s) {
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

int vrati_broj_registra(string s) {
    if (regex_match(s, regex("^r0"))) {
        return 0;
    } else if (regex_match(s, regex("^r1"))) {
        return 1;
    } else if (regex_match(s, regex("^r2"))) {
        return 2;
    } else if (regex_match(s, regex("^r3"))) {
        return 3;
    } else if (regex_match(s, regex("^r4"))) {
        return 4;
    } else if (regex_match(s, regex("^r5"))) {
        return 5;
    } else if (regex_match(s, regex("^(r6|sp)"))) {
        return 6;
    } else if (regex_match(s, regex("^(r7|pc)"))) {
        return 7;
    } else if (regex_match(s, regex("^(r8|psw)"))) {
        return 8;
    } else {
        return -1;
    }
}

string odredi_DestSrc_bajt(vector<string> niz, string instr) {
    string tmp;
    for (unsigned int i = 0; i < niz.size(); i++) {
        int broj_registra = vrati_broj_registra(niz[i]);
        if (broj_registra != -1) {
            tmp += to_string(broj_registra);
        } else {
            cout << "Greska na liniji:" << brojacLinije << "\n" << "Instrukcija " << instr << "; Nepostojeci registar: " << niz[i] << endl;
            exit(1);
        }
    }

    return tmp;
}

vector<string> vrati_niz_Registra_Iz_Linije(string linija) {
    vector<string> niz;         
    stringstream ss(linija);    // r0, r1
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, ',');  
        niz.push_back(tmp);     // niz = ["r0", "r1"]
    }

    return niz;
}

int konverzija_hexa_u_int(string hexaVrednost) {
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
