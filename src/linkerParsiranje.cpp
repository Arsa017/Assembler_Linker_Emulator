#include "../inc/linkerParsiranje.h"
#include "../inc/glob_podaciLinker.h"
#include "../inc/regex.h"


using namespace std;

void obradiFajl(ifstream* ulaz, int red_br_fajla) {
    int brojLinije = 0;
    string linija;
    while (getline(*ulaz, linija)) {

        // preskacemo prvih 6 linija u fajlu
        if (brojLinije < 6) {
            brojLinije++;
            continue;
        }
        
        parsirajLiniju(linija, red_br_fajla);
        brojLinije++;
    }
    // cout << "Fajl: " << red_br_fajla  << " je obradjen" << endl;  
}

void parsirajLiniju(string linija, int red_br_fajla) {
    std::replace(linija.begin(), linija.end(), '\t', ' ');          // zamena pojave svih tabulatora u liniji praznim znakom

    if(regex_search(linija, BELI_ZNAK)) {
            linija = regex_replace(linija, BELI_ZNAK, "");          // brisanje praznog znaka sa pocetka stringa; 
    }

    if (linija.size() == 0) {
        return;                     // prazan red
    }

    // linija je simbol iz tabele simbola
    if (regex_match(linija, SIMBOL)) {
        linija = regex_replace(linija, regex("\\s+"), " ");
        dodaj_u_tabelu_simbola(linija, red_br_fajla);
        return;
    }

    // preskacemo liniju koja sadrzi zvezdice
    if (regex_match(linija, ZVEZDICA)) {
        return;
    }

    // linija koja sadrzi naziv sekcije za predstojece rel. zapise
    if (regex_match(linija, RelZapis_SEKCIJA)) {                    // linija = #.rel.text
        linija = regex_replace(linija, regex("\\#\\.rel"), "");     // linija = .text
        curr_sekcijaL = linija;
        return;
    }

    // Offset            Tip         Simbol                     <--- ovu liniju treba da preskocimo
    if (regex_match(linija, RelZapis_HEADER)) {
        return;
    }

    // linija je zapis o realokaciji                            <--  mora da se proveri nakon RelZapis_HEADER; posto je ovo opsiti slucaj
    if (regex_match(linija, REL_ZAPIS)) {
        linija = regex_replace(linija, regex("\\s+"), " ");
        dodaj_u_tabelu_relZapisa(linija, red_br_fajla);
        return;
    }

    // linija koja sadrzi naziv sekcije za predstojeci masinski kod
    if (regex_match(linija, regex("\\#\\.?\\w+"))) {
        linija = regex_replace(linija, regex("\\#"), "");
        curr_sekcijaL = linija;
        return;
    }

    // ako linija nije prepoznata od strane nekog regex-a tada se sigurno radi o liniji koja sadrzi masinski kod
    parsiraj_masinski_kod(linija, red_br_fajla);
}

// obrada masinskog koda
void parsiraj_masinski_kod(string linija, int red_br_fajla) {                   // linija = 12 34 56 .... <-- raspored bajtova u liniji                                              
    linija.erase(remove(linija.begin(), linija.end(), ' '), linija.end());      // linija = 123456....

    // za svaku sekciju, svakog fajla parsiramo masinski kod koji cemo posle da objedinimo u jedan celovit string, gde je masin kod, rasporedjen na odgovarajucem mestu

    if (srdZaIspis.size() == 0) {
        // vektor zadrzaja za ispis je prazan; sto znaci da prvi put zelimo da dodamo masinski kod u njemu

        struct masKodLinker tmp;
        tmp.nazivSekcije = curr_sekcijaL;
        tmp.masinskiKod += linija;
        tmp.rb_Fajla = red_br_fajla;
        tmp.obradjen = 0;

        srdZaIspis.push_back(tmp);
    } else {
        // vektor vec ima neki sadrzaj pa prvo vrsimo proveru dal je deo masinskog koda curr_sekcijeL, za trenutno posmatrani fajl vec u njemu

        bool pronadjen = false;
        for (iter_masinKodL = srdZaIspis.begin(); iter_masinKodL != srdZaIspis.end(); iter_masinKodL++) {
            if ((*iter_masinKodL).nazivSekcije == curr_sekcijaL && (*iter_masinKodL).rb_Fajla == red_br_fajla) {
                pronadjen = true;
                (*iter_masinKodL).masinskiKod += linija;                            // dodajemo liniju parsiranog mas. koda za odgovarajucu sekciju odgovarajuceg fajla
            }
        }

        if (!pronadjen) {
            struct masKodLinker tmp;
            tmp.nazivSekcije = curr_sekcijaL;
            tmp.masinskiKod += linija;
            tmp.rb_Fajla = red_br_fajla;
            tmp.obradjen = 0;
            srdZaIspis.push_back(tmp);
        }
    }
}

// obrada rel. zapisa
void dodaj_u_tabelu_relZapisa(string linija, int red_br_fajla) {
    vector<string> niz;         
    stringstream ss(linija);    
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, ' ');  
        niz.push_back(tmp);     // niz = ["offset", "tip", "rb_simbola"];
    }

    int offset = konverzija_hexa_u_intL(niz[0]);
    string tip = niz[1];
    int rbSimbola = konverzija_hexa_u_intL(niz[2]);
    string nazivSekcije = curr_sekcijaL;
    int rbFajla = red_br_fajla;
    int obradjen = 0;

    // cout << offset << " " << tip << " " << rbSimbola << " " << nazivSekcije << " " << rbFajla << " " << obradjen << endl;

    RelZapisLinker* relZapis = new RelZapisLinker(offset, tip, rbSimbola, nazivSekcije, rbFajla, obradjen);
    lista_relZapisaL.push_back(relZapis);
}

// obrada simbola
void dodaj_u_tabelu_simbola(string linija, int red_br_fajla) {

    vector<string> niz;         
    stringstream ss(linija);    // r0, r1
    while(ss.good()) {
        string tmp;
        getline(ss, tmp, ' ');  
        niz.push_back(tmp);     // niz = ["Naziv", "sekcija", "vrednost", "vidljivost", "redni broj", "velicina"]
    }

 
    string naziv = niz[0];
    int sekcija = konverzija_hexa_u_intL(niz[1]);
    int vrednost = konverzija_hexa_u_intL(niz[2]);
    char vidljivost;
    if (niz[3] == "l") {
        vidljivost = 'l';
    } else {
        vidljivost = 'g';
    }
    int red_br = konverzija_hexa_u_intL(niz[4]);
    int velicina = konverzija_hexa_u_intL(niz[5]);
    int rb_fajla = red_br_fajla;

    // cout << naziv << " " << sekcija << " " << vrednost << " " << vidljivost << " " << red_br << " " << velicina << " " << rb_fajla << endl;

    // u tabelu simbola ne treba lokalne simbole
//    if (naziv != ".bss") {
    if (!(velicina == 0 && vidljivost == 'l')) {
        SimbolLinker* simbol = new SimbolLinker(naziv, sekcija, vrednost, vidljivost, red_br, velicina, red_br_fajla, false);
                tabela_simbolaL.push_back(simbol);
    }
//    }
    
}

