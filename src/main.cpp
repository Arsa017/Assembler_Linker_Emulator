#include <iostream>
#include <fstream>
#include <algorithm>    // za f-ju std::replace()
#include <string>
#include <regex>

#include "../inc/glob_podaci.h"
#include "../inc/obrada_linije.h"
#include "../inc/flink.h"
#include "../inc/regex.h"
#include "../inc/relZapis.h"
#include "../inc/simbol.h"
#include "../inc/nakon_prolaza.h"

using namespace std;

int main(int argc, char* argv[]) {

    string opcija = argv[1];
    if (argc != 4 || opcija != "-o") {
        cout << "Greska! Pokretanje asemblera se zadaje naredbom u formi: " << endl;
        cout << "./asembler -o izlaz.o ulaz.s" << endl;
        return -1;

    }

    string izlazniFajl = argv[2];
    string ulazniFajl = argv[3];

    try {
        izlaz.open("../test/" + izlazniFajl);
    } catch (exception& e) {
        cout << "Ne postoji izlazni fajl sa zadatim imenom!" << endl;
        return -1;
    }

    try {
        ulaz.open("../test/" + ulazniFajl);
    } catch (exception& e) {
        cout << "Ne postoji ulazni fajl sa zadatim imenom!" << endl;
        return -1;
    }

    Simbol* und_section = new Simbol("UND", 0, 0, 'l', 0, 0, true, false); // 0ta sekcija je sekcija nedefinisanih simbola
    lista_sekcija.push_back(und_section);

    while(getline(ulaz, linija)) {

        stringstream ss(linija);
        string noComment;
        getline(ss, noComment, '#');        // ciscenje od komentara; delimiter '#' govori f-ji da prekine citanje ulaznog strima cim naidje na njega
        linija = noComment;

        std::replace(linija.begin(), linija.end(), '\t', ' ');      // zamena pojave svih tabulatora u liniji praznim znakom

        if(regex_search(linija, BELI_ZNAK)) {
            linija = regex_replace(linija, BELI_ZNAK, "");          // brisanje praznog znaka sa pocetka stringa; 
        }

        if (regex_search(linija, END)) {
            obrada_end(linija);
            break;
        } else {
            obrada_linije();
        }
    
        brojacLinije++;
    }

    // obrada nakon 1. prolaza

    backpatching();

    formiranje_tabele_simbola();

    azuriranje_relZapisa();

    ispis_izlaza_u_fajl();
    
   
/*
    // cout << "Lista sekcija" << endl;
    // for (iter_simbol = lista_sekcija.begin(); iter_simbol != lista_sekcija.end(); iter_simbol++) {
    //     cout << (*iter_simbol)->naziv << " " << (*iter_simbol)->sekcija << " " << (*iter_simbol)->vrednost << " " << (*iter_simbol)->vidljivost << " " << (*iter_simbol)->redniBroj << " " << (*iter_simbol)->velicina << " " << (*iter_simbol)->definisan << " " << (*iter_simbol)->extrn << endl;
    // }
    // cout << "-------------------------------------------------------" << endl;
    // for(iter_masinKod = sadrzajZaIspis.begin(); iter_masinKod != sadrzajZaIspis.end(); iter_masinKod++) {
    //     cout << "Sekcija: " << (*iter_masinKod).nazivSekcije << " Sadrzaj: " << (*iter_masinKod).masinskiKod << endl;      
    // }
    // cout << "-------------------------------------------------------" << endl;
    // cout << "Lista simbola" << endl; 
    // for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
    //     cout << (*iter_simbol)->naziv << " " << (*iter_simbol)->sekcija << " " << (*iter_simbol)->vrednost << " " << (*iter_simbol)->vidljivost << " " << (*iter_simbol)->redniBroj << " " << (*iter_simbol)->velicina << " " << (*iter_simbol)->definisan << " " << (*iter_simbol)->extrn << endl;
    // }
    // cout << "-------------------------------------------------------" << endl;
    // cout << "Realokacioni zapisi" << endl; 
    // for (iter_relZapis = lista_relZapisa.begin(); iter_relZapis != lista_relZapisa.end(); iter_relZapis++) {
    //     cout << (*iter_relZapis)->offset << " " << (*iter_relZapis)->tip << " " << (*iter_relZapis)->red_br_simbola << " " << (*iter_relZapis)->red_br_sekcije << " " << (*iter_relZapis)->vidljivost_simbola << endl;
    // }
    // cout << "-------------------------------------------------------" << endl;
    // cout << "Tabela obracanja unapred" << endl;
    // for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
    //     if (!(*iter_simbol)->lista.empty()) {
    //         for (iter_flink = (*iter_simbol)->lista.begin(); iter_flink != (*iter_simbol)->lista.end(); iter_flink++) {
    //             cout << "simbol:" << (*iter_simbol)->naziv << " " << (*iter_flink)->pozicija << " " << (*iter_flink)->sekcija << " " << (*iter_flink)->operacija << " " << (*iter_flink)->br_bajtova << endl;
    //         }
    //     } else {
    //         cout << "Tabela obracanja unapred za simbol: " << (*iter_simbol)->naziv << " je prazna!" << endl;
    //     }
    // }
    // cout << "-------------------------------------------------------" << endl;

    // cout << "Tabela simbola" << endl;
    // for (iter_simbol = tabela_simbola.begin(); iter_simbol != tabela_simbola.end(); iter_simbol++) {
    //     cout << (*iter_simbol)->naziv << " " << (*iter_simbol)->sekcija << " " << (*iter_simbol)->vrednost << " " << (*iter_simbol)->vidljivost << " " << (*iter_simbol)->redniBroj << " " << (*iter_simbol)->velicina << " " << (*iter_simbol)->definisan << " " << (*iter_simbol)->extrn << endl;
    // }
    // cout << "-------------------------------------------------------" << endl;
*/

    // zatvaranje ulaznog i izlaznog fajla
    ulaz.close();
    izlaz.close();

    return 0;
}

// kompajlovanje
// g++ main.cpp flink.cpp glob_podaci.cpp obrada_linije.cpp regex.cpp relZapis.cpp simbol.cpp nakon_prolaza.cpp  -o asembler

// pokretanje
// ./asembler -o izlaz.o ulaz.s