#include <iostream>
#include <fstream>
#include <algorithm>    // za f-ju std::replace()
#include <string>
#include <regex>

#include "../inc/glob_podaciLinker.h"
#include "../inc/linkerParsiranje.h"
#include "../inc/simbolLinker.h"
#include "../inc/relZapisLinker.h"
#include "../inc/obradaLinker.h"

using namespace std;

int main(int argc, char* argv[]) {

    int brArgumenata = argc;
    
    // ifstream ulazLinkera;
    // ofstream izlaz;             // output file stream
    
    string opcija1 = argv[1];
    string opcija2 = argv[2];

    if (opcija1 != "-hex" || opcija2 != "-o") {
        cout << "Greska! Pokretanje linkera se zadaje naredbom u formi: " << endl;
        cout << "./linker -hex -o mem_content.hex ulaz1.o ulaz2.o" << endl;
        return -1;
    }

    string izlazniFajl = argv[3];
    
    vector<string> ulazniFajl;
    for(int i = 4; i < argc; i++) {
        ulazniFajl.push_back(argv[i]);
    }

    // otvaranje fajla za ispis -> mem_content.hex
    try {   
        izlaz.open("../test/" + izlazniFajl);                                           
    } catch (exception& e) {
        cout << "Ne postoji izlazni fajl sa zadatim imenom!" << endl;
        return -1;
    }

    // otvaranje fajla za citanje -> mem_content.hex
    try {   
        int red_br_fajla = 0;
        for(int i = 4; i < argc; i++) {
            ulazLinkera.open("../test/" + ulazniFajl[red_br_fajla]);
            // cout << "Obrada ulaznog fajla " << ulazniFajl[red_br_fajla] << endl;

            // parsiranje fajla
            obradiFajl(&ulazLinkera, red_br_fajla);

            ulazLinkera.close();
            red_br_fajla++;
        }   
    } catch (exception& e) {
        cout << "Ne postoji izlazni fajl sa zadatim imenom!" << endl;
        return -1;
    }

    // Obrada Linkera nakon parsiranja

    // ucitavanje sekcija, obrada i azuriranje informacija o sekcijama 

    // cout << "Tabela simbola PRE linkovanja sekcija: " << endl;
    // for (iter_simbolL = tabela_simbolaL.begin(); iter_simbolL != tabela_simbolaL.end(); iter_simbolL++) {
    //     cout << (*iter_simbolL)->naziv << " " << (*iter_simbolL)->sekcija << " " << (*iter_simbolL)->vrednost << " " << (*iter_simbolL)->vidljivost << " " 
    //      << (*iter_simbolL)->redniBroj << " " << (*iter_simbolL)->velicina << " " << (*iter_simbolL)->red_br_fajla << " " << (*iter_simbolL)->obradjen << endl;
    // }
    // cout << "-----------------------------------" << endl;

    linkovanje_sekcija();    

    // cout << "Tabela simbola NAKON linkovanja sekcija: " << endl;
    // for (iter_simbolL = tabela_simbolaL.begin(); iter_simbolL != tabela_simbolaL.end(); iter_simbolL++) {
    //     cout << (*iter_simbolL)->naziv << " " << (*iter_simbolL)->sekcija << " " << (*iter_simbolL)->vrednost << " " << (*iter_simbolL)->vidljivost << " " 
    //      << (*iter_simbolL)->redniBroj << " " << (*iter_simbolL)->velicina << " " << (*iter_simbolL)->red_br_fajla << " " << (*iter_simbolL)->obradjen << endl;
    // }
    // cout << "-----------------------------------" << endl;             

    azuriranje_globalnih_simbola();

    // cout << "Finalna tabela simbola: " << endl;
    // for (iter_finalTable = final_tabela_simbolaL.begin(); iter_finalTable != final_tabela_simbolaL.end(); iter_finalTable++) {
    //     cout << (*iter_finalTable)->naziv << " " << (*iter_finalTable)->sekcija << " " << (*iter_finalTable)->vrednost << " " << (*iter_finalTable)->vidljivost << " "
    //     << (*iter_finalTable)->redniBroj << " " << (*iter_finalTable)->velicina << endl;
    // }

    azuriranje_rel_zapisa();

    // cout << "Tabela rel. zapisa POSLE azurirnaja: " << endl;
    // for (iter_relZapisL = lista_relZapisaL.begin(); iter_relZapisL != lista_relZapisaL.end(); iter_relZapisL++) {
    //     cout << (*iter_relZapisL)->offset << " " << (*iter_relZapisL)->tip << " " << (*iter_relZapisL)->red_br_simbola << " " << (*iter_relZapisL)->nazivSekcije << " " << (*iter_relZapisL)->red_br_fajla  << " " << (*iter_relZapisL)->obradjen << endl;
    // }
    // cout << "-----------------------------------" << endl;

    povezivanje_masinskog_koda();

    // izlaz << "Pre razresavanja rel. zapisa" << endl;
    // ispis_u_izlazni_fajl();

    razresavanje_rel_zapisa();                              // i prepravljanje sadrzaja u masinskom kodu

    // izlaz << endl;
    // izlaz << "Posle razresavanja rel. zapisa" << endl;
    
    ispis_u_izlazni_fajl();



    // cout << "Tabela simbola: " << endl;
    // for (iter_simbolL = tabela_simbolaL.begin(); iter_simbolL != tabela_simbolaL.end(); iter_simbolL++) {
    //     cout << (*iter_simbolL)->naziv << " " << (*iter_simbolL)->sekcija << " " << (*iter_simbolL)->vrednost << " " << (*iter_simbolL)->vidljivost << " " 
    //      << (*iter_simbolL)->redniBroj << " " << (*iter_simbolL)->velicina << " " << (*iter_simbolL)->red_br_fajla << " " << (*iter_simbolL)->obradjen << endl;
    // }
    // cout << "-----------------------------------" << endl;

    // cout << "Finalna tabela simbola: " << endl;
    // for (iter_finalTable = final_tabela_simbolaL.begin(); iter_finalTable != final_tabela_simbolaL.end(); iter_finalTable++) {
    //     cout << (*iter_finalTable)->naziv << " " << (*iter_finalTable)->sekcija << " " << (*iter_finalTable)->vrednost << " " << (*iter_finalTable)->vidljivost << " "
    //     << (*iter_finalTable)->redniBroj << " " << (*iter_finalTable)->velicina << endl;
    // }

    // cout << "Tabela rel. zapisa posle azurirnaja: " << endl;
    // for (iter_relZapisL = lista_relZapisaL.begin(); iter_relZapisL != lista_relZapisaL.end(); iter_relZapisL++) {
    //     cout << (*iter_relZapisL)->offset << " " << (*iter_relZapisL)->tip << " " << (*iter_relZapisL)->red_br_simbola << " " << (*iter_relZapisL)->nazivSekcije << " " << (*iter_relZapisL)->red_br_fajla  << " " << (*iter_relZapisL)->obradjen << endl;
    // }
    // cout << "-----------------------------------" << endl;

    // cout << "Masinski kod nakon spajanja:" << endl;
    // for (iter_masinKodL = srdZaIspis.begin(); iter_masinKodL != srdZaIspis.end(); iter_masinKodL++) {
    //     cout << (*iter_masinKodL).nazivSekcije << " " << (*iter_masinKodL).rb_Fajla << " " << (*iter_masinKodL).obradjen <<  endl;
    //     cout << (*iter_masinKodL).masinskiKod << endl;     
    // }
    // cout << "-----------------------------------" << endl;


    // cout << "HEX kod NAKON razresavanja vrednosti:" << endl;
    // cout << hex_kod << endl;
    // cout << "-----------------------------------" << endl;


    return 0;
}


// kompajlovanje
// g++ linkerMain.cpp linkerParsiranje.cpp simbolLinker.cpp relZapisLinker.cpp glob_podaciLinker.cpp regex.cpp obradaLinker.cpp -o linker

// pokretanje
// ./linker -hex -o mem_content.hex izlaz.o izlaz1.o izlaz2.o