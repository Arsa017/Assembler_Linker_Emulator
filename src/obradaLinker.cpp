#include "../inc/obradaLinker.h"
#include "../inc/glob_podaciLinker.h"
#include "../inc/regex.h"
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

// spajanje sekcija u jednu celinu
void linkovanje_sekcija() {
    for (iter_simbolL = tabela_simbolaL.begin(); iter_simbolL != tabela_simbolaL.end(); iter_simbolL++) {
        
        if ((*iter_simbolL)->velicina > 0 && !(*iter_simbolL)->obradjen) {
            redBrSekcijaFinal++;
            redBrFinal++;
            (*iter_simbolL)->vrednost = sizeCnt;
            SimbolLinker* finalSym = new SimbolLinker((*iter_simbolL)->naziv, redBrSekcijaFinal, sizeCnt, 'l', redBrFinal, 0, -1, true);    
            sizeCnt += (*iter_simbolL)->velicina;

            // prpvera da li je poslednji elemnet
            if (*iter_simbolL != tabela_simbolaL.back()) {
                // obrada sledbenika
                // cout << (*iter_simbolL)->naziv << " **" << endl;
                pomIter_simbolL = std::next(iter_simbolL);
                for ( ;pomIter_simbolL != tabela_simbolaL.end(); pomIter_simbolL++) {
                    // obrada sledbenika na koje ukazuje pomIterator
                    if ((*pomIter_simbolL)->velicina > 0 && !(*pomIter_simbolL)->obradjen) {
                        // cout << (*pomIter_simbolL)->naziv << " " <<  endl;
                        
                        if ((*pomIter_simbolL)->naziv == (*iter_simbolL)->naziv) {
                            (*pomIter_simbolL)->vrednost = sizeCnt;
                            sizeCnt += (*pomIter_simbolL)->velicina;
                            (*pomIter_simbolL)->obradjen = true;
                        }
                    }
                }

                finalSym->velicina = sizeCnt - (*iter_simbolL)->vrednost;       // obradili smo i azurirali sve sledbenike; upisujemo velicinu finalne sekcije 
                final_tabela_simbolaL.push_back(finalSym);                      // i dodajemo je u finalnu tabelu simbola
                (*iter_simbolL)->obradjen = true;

                // cout << "-------------------------" << endl;           
            } else {
                // cout << "Obrada poslednjeg elementa" << endl;
                // cout << (*iter_simbolL)->naziv << " --->  je poslednji" << endl;

                finalSym->velicina = sizeCnt - (*iter_simbolL)->vrednost;
                final_tabela_simbolaL.push_back(finalSym);
                (*iter_simbolL)->obradjen = true;
            }
        }
    }
}

void azuriranje_globalnih_simbola() {
    for (iter_simbolL = tabela_simbolaL.begin(); iter_simbolL != tabela_simbolaL.end(); iter_simbolL++) {
        
        if ((*iter_simbolL)->vidljivost == 'g') {                                       // izdvajanje simbola za obradu iz tabela simbola
            if ((*iter_simbolL)->sekcija != 0) {
                // obrada definisanih simbola

                // 1. provera da li vec postoji u finalnoj tabeli simbola i ako postoji --> GRESKA, DVOSTRUKA definicija simbola
                bool dvostruka_def_simbola = false;
                dvostruka_def_simbola = nadji_Simbol_u_Finalnoj_tabeli_simbola((*iter_simbolL)->naziv);
                if (dvostruka_def_simbola) {
                    cout << "Greska Linker! Visestruka definicija simbola: " << (*iter_simbolL)->naziv << endl;
                    exit(1);
                }

                // azuriranje rednog broja sekcije i vrednosti simbola i dodavanje u FINALNU tabelu simbola;
                redBrFinal++;
                SimbolLinker* finalSym = new SimbolLinker((*iter_simbolL)->naziv, 0, 0, 'g', redBrFinal, 0, -1, true);

                // azuriranje rednog broja sekcije
                int tmpRbSekcije = azuriraj_finalRb_sekcije_za_simbol((*iter_simbolL)->sekcija, (*iter_simbolL)->red_br_fajla);
                finalSym->sekcija = tmpRbSekcije;

                // azuriranje vrednosti 
                int tmpVrednost = odredi_finalnu_vrednost((*iter_simbolL)->sekcija, (*iter_simbolL)->red_br_fajla, (*iter_simbolL)->vrednost);
                finalSym->vrednost = tmpVrednost;

                // cout << finalSym->naziv << " " << finalSym->sekcija << " " << finalSym->vrednost << " " << finalSym->vidljivost << " " << finalSym->redniBroj << endl;                
                final_tabela_simbolaL.push_back(finalSym);
            } else {
                // obrada nedefinisanih simbola
                lista_undSimbola.push_back(*iter_simbolL);                              // nedefinisan simbol dodajemo u listu UND simbola
            }
        }
    }

    // provera da li su externi simboli na kraju definisani; 
    // ako koristimo neki externi simbol koji nije u FINALNOJ tabeli simbola, prijavljujemo gresku   
    for (iter_undList = lista_undSimbola.begin(); iter_undList != lista_undSimbola.end(); iter_undList++) {
        // cout << (*iter_undList)->naziv << " " << (*iter_undList)->red_br_fajla << endl;
        // cout << "Trazi nedefinisan simbol: " << (*iter_undList)->naziv << endl;
        bool nadjen = true;
        nadjen = nadji_Simbol_u_Finalnoj_tabeli_simbola((*iter_undList)->naziv);
        if (!nadjen) {
            cout << "Greska linker! Nerazresen simbol: " << (*iter_undList)->naziv << endl;
            exit(1);
        }
    }

}

bool nadji_Simbol_u_Finalnoj_tabeli_simbola(string nazivSimbola) {
    // cout << "Trazimo simbol: " << nazivSimbola << endl;
    bool pronadjen = false;
    for (iter_finalTable = final_tabela_simbolaL.begin(); iter_finalTable != final_tabela_simbolaL.end(); iter_finalTable++) {
        if ((*iter_finalTable)->naziv == nazivSimbola) {
            pronadjen = true;
            break;                                              // pronasli smo simbol pa prekidamo iteraciju
        }
    }   
    return pronadjen;
}

int azuriraj_finalRb_sekcije_za_simbol(int currRbSekcije, int rbFajla) {
    // cout << currRbSekcije << " " << rbFajla << endl;
    string nazivSekcije;
    for (pomIter_simbolL = tabela_simbolaL.begin(); pomIter_simbolL != tabela_simbolaL.end(); pomIter_simbolL++) {
        if ((*pomIter_simbolL)->sekcija == currRbSekcije && (*pomIter_simbolL)->red_br_fajla == rbFajla) {
            nazivSekcije = (*pomIter_simbolL)->naziv;
            break;                                              // mora da se izadje iz petlje kad se naidje na odgovarajucu sekciju
        }
    }
    // cout << nazivSekcije << endl;
    int finalniRedniBroj;
    for (pomIter_simbolL = final_tabela_simbolaL.begin(); pomIter_simbolL != final_tabela_simbolaL.end(); pomIter_simbolL++) {
        if ((*pomIter_simbolL)->naziv == nazivSekcije) {
            finalniRedniBroj = (*pomIter_simbolL)->sekcija;
            break;
        }
    }
    // cout << finalniRedniBroj << endl;
    return finalniRedniBroj;
}

int odredi_finalnu_vrednost(int currRbSekcje, int rbFajla, int currVrednost) {
    // cout << currRbSekcje << " " << rbFajla << " " << currVrednost << endl;
    // vrednost globalnog simbola treba uvecati za pomeraj sekcije iz ulaznog fajla u KOJOJ je on DEFINISAN. tj za pocetnu adresu sekcije nakon razmestanja

    int vrednost = currVrednost;
    // sekciju u kojoj je simbol definisan
    for (pomIter_simbolL = tabela_simbolaL.begin(); pomIter_simbolL != tabela_simbolaL.end(); pomIter_simbolL++) {
        if ((*pomIter_simbolL)->sekcija == currRbSekcje && (*pomIter_simbolL)->red_br_fajla == rbFajla) {
            vrednost += (*pomIter_simbolL)->vrednost;
            break;                                                  // izlazimo iz petlje nakon sto smo odredili finalnu frednost
        }
    }

    // cout << "Vrednost simbola nakon linkovanja: " << vrednost << endl;
    return vrednost;
}

void azuriranje_rel_zapisa() {
    for (iter_relZapisL = lista_relZapisaL.begin(); iter_relZapisL != lista_relZapisaL.end(); iter_relZapisL++) {
        // azuriranje offset-a rel. zapisa
        int pomeraj_sekcije = nadji_pomeraj_sekcije((*iter_relZapisL)->nazivSekcije, (*iter_relZapisL)->red_br_fajla);
        (*iter_relZapisL)->offset += pomeraj_sekcije;

        // azuriranje rednog broja simbola na koji se rel. zapis odnosi
        int redniBroj = rb_simbola_za_rel_zapis((*iter_relZapisL)->red_br_simbola, (*iter_relZapisL)->red_br_fajla);
        (*iter_relZapisL)->red_br_simbola = redniBroj;
    }
}

int rb_simbola_za_rel_zapis(int rbSimbola, int rbFajla) {
    string nazivSimbola;
    int trazeni_redni_broj;
    for (pomIter_simbolL = tabela_simbolaL.begin(); pomIter_simbolL != tabela_simbolaL.end(); pomIter_simbolL++) {
        if ((*pomIter_simbolL)->redniBroj == rbSimbola && (*pomIter_simbolL)->red_br_fajla == rbFajla) {
            nazivSimbola = (*pomIter_simbolL)->naziv;
            break;
        }
    }
    // za nadjeni naziv Simbola dohvatamo njegov redni broj iz Finalne tabele simbola
    for (pomIter_simbolL = final_tabela_simbolaL.begin(); pomIter_simbolL != final_tabela_simbolaL.end(); pomIter_simbolL++) {
        if ((*pomIter_simbolL)->naziv == nazivSimbola) {
            trazeni_redni_broj = (*pomIter_simbolL)->redniBroj;
            break;
        }
    }
    return trazeni_redni_broj;
}

int nadji_pomeraj_sekcije(string nazivSekije, int rbFajla) {
    int offset = 0;
    // cout << "Naziv sekcije: " << nazivSekije << endl;
    for (pomIter_simbolL = tabela_simbolaL.begin(); pomIter_simbolL != tabela_simbolaL.end(); pomIter_simbolL++) {
        if ((*pomIter_simbolL)->naziv == nazivSekije && (*pomIter_simbolL)->red_br_fajla == rbFajla) {
            // cout << "*** ULAZI ***" << endl;
            offset = (*pomIter_simbolL)->vrednost;
            break;
        }
    }
    return offset;
}

void povezivanje_masinskog_koda() {
    for (iter_masinKodL = srdZaIspis.begin(); iter_masinKodL != srdZaIspis.end(); iter_masinKodL++) {
        // proveravamo da li smo obradali m. kod neke sekcije kako ga ne bi duplirali u izlazu
        if (!(*iter_masinKodL).obradjen) {
            hex_kod += (*iter_masinKodL).masinskiKod;
            (*iter_masinKodL).obradjen = true;

            // spajanje masinskog koda za sve istoimene sekcije i nadovezivanje u hex_kod
            for (pomIterMK = srdZaIspis.begin(); pomIterMK != srdZaIspis.end(); pomIterMK++) {
                if ((*iter_masinKodL).nazivSekcije == (*pomIterMK).nazivSekcije && !(*pomIterMK).obradjen) {
                    // hex_kod += " ";
                    hex_kod += (*pomIterMK).masinskiKod;
                    (*pomIterMK).obradjen = true;
                }
            }

        }
        // hex_kod += " ";
    }
}

void razresavanje_rel_zapisa() {
    int offset = 0;
    int vrednost = 0;

    for (iter_relZapisL = lista_relZapisaL.begin(); iter_relZapisL != lista_relZapisaL.end(); iter_relZapisL++) {
        offset = (*iter_relZapisL)->offset;
        vrednost = vrednost_za_ugradnju((*iter_relZapisL)->red_br_simbola, (*iter_relZapisL)->red_br_fajla);

        
        int index = offset * 2;                                         // pozicija u hex_kodu koju treba prepraviti
        if((*iter_relZapisL)->tip == "R_386_16") {
            // prepravka zapisa o realokaciji za apsolutno adresiranje         

            int oldVal = nadji_staru_vrednost(index);
            // cout << "R_386_16   --->  " << "Vrednost: " << vrednost  << " Stara vrednost: " << oldVal << " *Adresno polje: " << offset << endl;
            int newVal = oldVal + vrednost;                                        
            ugradi_novu_vrednost(index, newVal);
        } 
        else {
            // prepravka zapisa o realokaciji za PC-relativno adresiranje

            int oldVal = nadji_staru_vrednost(index);
            // cout << "R_386_PC16   --->  "  << "Vrednost: " << vrednost  << " Stara vrednost: " << oldVal << " Adresno polje: " << offset << endl;
            int newVal = oldVal + (vrednost - offset);                             
            ugradi_novu_vrednost(index, newVal);
        }
    }

}

void ugradi_novu_vrednost(int index, int vrednost) {
    string str;
    bool negativan;
    if (vrednost >= 0) {
        negativan = false;
    } else {
        negativan = true;
    }

    if (!negativan) {
        str = decimal_u_oznaceni_hexaL(vrednost);               // nema odsecanja posto je pozitivan broj
    } else {
        string tmp = decimal_u_oznaceni_hexaL(vrednost);        // -4  ===> FF FF FF FC         negativan broj
        str = negativni_wordL(tmp);                             // FF FF FF FC  ===>   FF FC
    }

    string le_str = little_endian_wordL(str);                   // FC FF
    // cout << "Vrednost za ugradnju " << le_str << endl;
    // cout << endl;
    hex_kod[index++] = le_str[0];                              // 0 -> F
    hex_kod[index++] = le_str[1];                              // 1 -> C
    hex_kod[index++] = le_str[2];                              // 2 -> F
    hex_kod[index]   = le_str[3];                              // 3 -> F
}

int nadji_staru_vrednost(int indeks) {
    int tmpIndeks1 = indeks;         
    int nadjenaVrednost;
                                                // na poziciji index => FE FF
    char c0 = hex_kod[tmpIndeks1++];            // c0 = F
    char c1 = hex_kod[tmpIndeks1++];            // c1 = E
    char c2 = hex_kod[tmpIndeks1++];            // c2 = F
    char c3 = hex_kod[tmpIndeks1++];            // c3 = F

    string value = "";
    value += c2; 
    value += c3; 
    value += c0;
    value += c1;

    if (c2 == 'f') {
        // negativan broj
        string negativanBroj = "ffff";
        negativanBroj += c2;
        negativanBroj += c3;
        negativanBroj += c0;
        negativanBroj += c1;

        unsigned int x;
        std::stringstream ss;
        ss << std::hex << negativanBroj;
        ss >> x;

        nadjenaVrednost = x;
    } else {
        // pozitivan broj
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << value;
        ss >> x;
        nadjenaVrednost = x;
    }

    return nadjenaVrednost;
}

int vrednost_za_ugradnju(int rbSimbola, int rbFajla) {
    // prolazimo kroz finalnu tabelu simbola i u zavisnosti od toga da li je simbol ili sekcija,
    // vrsimo obradu i vracamo vrednost koju treba ugraditi u masinski kod
    int value = 0;
    for (iter_simbolL = final_tabela_simbolaL.begin(); iter_simbolL != final_tabela_simbolaL.end(); iter_simbolL++) {
        if ((*iter_simbolL)->redniBroj == rbSimbola) {
            if ((*iter_simbolL)->vidljivost == 'l') {
                // vracamo vrednost sekcije; odnosno pomeraj pocetka sekcije za zadati redni broj fajla (nakon spajanja sekcija)
                value = nadji_vrednost_sekcije((*iter_simbolL)->naziv, rbFajla);
            } else {
                // vracamo vrednost globalnog simbola iz finalne tabele simbola
                value = (*iter_simbolL)->vrednost;
            }
            break;
        }
    }
    return value;
}

int nadji_vrednost_sekcije(string nazivSekcije, int rbFajla) {
    int val = 0;
    for (pomIter_simbolL = tabela_simbolaL.begin(); pomIter_simbolL != tabela_simbolaL.end(); pomIter_simbolL++) {
        if ((*pomIter_simbolL)->naziv == nazivSekcije && (*pomIter_simbolL)->red_br_fajla == rbFajla) {
            // nadjena vrednost, tj. ofset sekcije u ciljnom fajlu
            val = (*pomIter_simbolL)->vrednost;
            break;
        }
    }
    return val;
}

void ispis_u_izlazni_fajl() {
    string hex_kod_tmp;
    hex_kod_tmp = formatiranje_hex_koda(hex_kod);
    int adresa = 0;

    string adr;
    for (long i = 0; i < hex_kod_tmp.length(); i++) {
        if ((i % 24) == 0) {           
            izlaz << "\n";
            string adr = decimal_u_oznaceni_hexaL(adresa);
            adr = hex_u_cetiri_bajta(adr);
            izlaz << adr << ": ";
            adresa += 8;
        }
        izlaz << hex_kod_tmp[i];
    }
}

// dodavanje razmaka
string formatiranje_hex_koda(string str) {
    string tmp = "";
    for (unsigned int i = 0; i < str.size(); i++) {
        tmp += str[i];
        if ((i % 2) != 0) {
            tmp += " ";
        }
    }
    return tmp;
}