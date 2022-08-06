#include "../inc/nakon_prolaza.h"
#include <cctype>
#include <cstdlib>
#include <iomanip>          // za setwitdth f-ju, tj setw

using namespace std;

void backpatching() {

    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        // proveravamo da li je za simbol bilo obracanja unapred
        if (!(*iter_simbol)->lista.empty()) {
            // provera da li je simbol definisan
            if ((*iter_simbol)->definisan) {
                // backpatching za definisane simbole   
                for (iter_flink = (*iter_simbol)->lista.begin(); iter_flink != (*iter_simbol)->lista.end(); iter_flink++) {
                    if ((*iter_flink)->operacija == 0) {
                        // apsolutno adresiranje
                        if ((*iter_simbol)->vidljivost == 'l') {
                            // lokalan simbol
                            RelZapis* relokZapis = new RelZapis((*iter_flink)->pozicija, "R_386_16", (*iter_simbol)->sekcija, (*iter_flink)->sekcija, 'l');     // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
                            lista_relZapisa.push_back(relokZapis);

                            string vrednost = decimal_u_oznaceni_hexa((*iter_simbol)->vrednost);                // 4    ==>  0004
                            string le_str = little_endian_word(vrednost);                                       // 0004 ==>  0400

                            int index = (*iter_flink)->pozicija * 2;                                            // indeks za prepravku
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[0];        // 0 -> 0   
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[1];        // 0 -> 4
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[2];        // 0 -> 0
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index] = le_str[3];          // 0 -> 0    
                        } else {
                            // globalan simbol
                            RelZapis* relokZapis = new RelZapis((*iter_flink)->pozicija, "R_386_16", (*iter_simbol)->redniBroj, (*iter_flink)->sekcija, 'g');   // simbol globalan pa se rel. zapis referencira u odnosu na njega
                            lista_relZapisa.push_back(relokZapis);
                        } 
                    } else {
                        // PC-relativno adresiranje
                        if ((*iter_simbol)->vidljivost == 'l') {
                            // lokalan simbol
                            if ((*iter_simbol)->sekcija == (*iter_flink)->sekcija) {
                                // lokalan simbol definisan u istoj sekciji kao instrukcija u kojoj se koristi; nema realokacioni zapis; razlika x-y je konstantna nezavisno od procesa linkovanja
                                // x - y = x - (* + 2)  ==> x - * - 2;      ----> x je vrednost simbola; * je adresno polje, odnosno loc_counter; a -2 je pomeraj od sledece instrukcije do adresnog polja
                                int offset = (*iter_simbol)->vrednost - (*iter_flink)->pozicija - 2;
                                string str;
                                bool negativan;
                                if (offset >= 0) {
                                    negativan = false;              // pozitivan offset
                                } else {
                                    negativan = true;               // negativan offset
                                }

                                if (!negativan) {
                                    str = decimal_u_oznaceni_hexa(offset);              // nema odsecanja posto je pozitivan broj
                                } else {
                                    string tmp = decimal_u_oznaceni_hexa(offset);       // -4  ===> FF FF FF FC         negativan broj
                                    str = negativni_word(tmp);                          // FF FF FF FC  ===>   FF FC 
                                }

                                string le_str = little_endian_word(str);                // FF FC   ===>  FC FF;  0005 ====> 0500
                                int index = (*iter_flink)->pozicija * 2;                                   // index za prepravku
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[0];    // 0 -> 0   
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[1];    // 0 -> 5
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[2];    // 0 -> 0
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index] = le_str[3];      // 0 -> 0 
                            } 
                            else {
                                // lokalan simbol definisan u nekoj drugoj sekciji 
                                RelZapis* relokZapis = new RelZapis((*iter_flink)->pozicija, "R_386_PC16", (*iter_simbol)->sekcija, (*iter_flink)->sekcija, 'l'); // rel. zapis se pravi u odnosu na SEKCIJU u kojoj je simbol DEFINISAN
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
                                // cout << (*iter_simbol)->vrednost << " " << (*iter_flink)->pozicija <<  endl;
                                // cout << offset << endl; 
                                if (!negativan) {
                                    str = decimal_u_oznaceni_hexa(offset);              // nema odsecanja posto je pozitivan broj
                                } else {
                                    string tmp = decimal_u_oznaceni_hexa(offset);       // -4  ===> FF FF FF FC         negativan broj
                                    str = negativni_word(tmp);                          // FF FF FF FC  ===>   FF FC 
                                }

                                string le_str = little_endian_word(str);                // FF FC   ===>  FC FF;  0005 ====> 0500
                                int index = (*iter_flink)->pozicija * 2;                // index za prepravku
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[0];    // 0 -> 0   
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[1];    // 0 -> 5
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[2];    // 0 -> 0
                                sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index] = le_str[3];      // 0 -> 0 
                            }
                        } 
                        else {
                            // globalan simbol
                            RelZapis* relokZapis = new RelZapis((*iter_flink)->pozicija, "R_386_PC16", (*iter_simbol)->redniBroj, (*iter_flink)->sekcija, 'g');   // simbol globalan pa se rel. zapis referencira u odnosu na njega
                            lista_relZapisa.push_back(relokZapis);

                            // ugradjujemo -2 u masinski kod; sto nam je pomeraj od sledece instrukcije do adresnog polja
                            string tmp = decimal_u_oznaceni_hexa(-2);        // -2          ===> FF FF FF FE
                            string str = negativni_word(tmp);                // FF FF FF FE ===> FF FE
                            string le_str = little_endian_word(str);         // ff fe => fe ff

                            int index = (*iter_flink)->pozicija * 2;                                        // index za prepravku
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[0];    // 0 -> f   
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[1];    // 0 -> e
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[2];    // 0 -> f
                            sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index] = le_str[3];      // 0 -> f 
                        }
                    }
                }
            } 
            else {
                // backpatching za nedefinisane simbole (EKSTERNI simboli koji se uvoze)
                for (iter_flink = (*iter_simbol)->lista.begin(); iter_flink != (*iter_simbol)->lista.end(); iter_flink++) {
                    if ((*iter_flink)->operacija == 0) {
                        // apsolutno adresiranje    
                        RelZapis* relokZapis = new RelZapis((*iter_flink)->pozicija, "R_386_16", (*iter_simbol)->redniBroj, (*iter_flink)->sekcija, 'g');       // simbol globalan pa se rel. zapis referencira u odnosu na njega
                        lista_relZapisa.push_back(relokZapis);
                    } else {
                        // PC-relativno adresiranje
                        RelZapis* relokZapis = new RelZapis((*iter_flink)->pozicija, "R_386_PC16", (*iter_simbol)->redniBroj, (*iter_flink)->sekcija, 'g');     // simbol globalan pa se rel. zapis referencira u odnosu na njega
                        lista_relZapisa.push_back(relokZapis);

                        // ugradjujemo -2 u masinski kod; sto nam je pomeraj od sledece instrukcije do adresnog polja
                        string tmp = decimal_u_oznaceni_hexa(-2);        // -2          ===> FF FF FF FE
                        string str = negativni_word(tmp);                // FF FF FF FE ===> FF FE
                        string le_str = little_endian_word(str);         // ff fe => fe ff

                        // 00 11 22 33 44 55      # preslikavanja bajt u indeks
                        // 01 23 45 67 89 
                        int index = (*iter_flink)->pozicija * 2;                                        // index za prepravku
                        sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[0];    // 0 -> f   
                        sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[1];    // 0 -> e
                        sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index++] = le_str[2];    // 0 -> f
                        sadrzajZaIspis[(*iter_flink)->sekcija - 1].masinskiKod[index] = le_str[3];      // 0 -> f                       
                    }
                }   
            }
        } 
    }
}

void formiranje_tabele_simbola() {
    int broj_sekcija = lista_sekcija.size();

    // dodavanje sekcija u tabelu simbola ---> iz liste sekcija
    for (iter_simbol = lista_sekcija.begin(); iter_simbol != lista_sekcija.end(); iter_simbol++) {
        tabela_simbola.push_back(*iter_simbol);
    }
    
    // dodavanje simbola u tabelu simbola ---> iz liste simbola
    for (iter_simbol = lista_simbola.begin(); iter_simbol != lista_simbola.end(); iter_simbol++) {
        (*iter_simbol)->redniBroj = (*iter_simbol)->redniBroj + (broj_sekcija - 1);
        tabela_simbola.push_back(*iter_simbol);
    }
}

// azuriranje rel. zapisa za globalne simbole, zbog promene njihovog rednog broja nakon formiranja tabele simbola
void azuriranje_relZapisa() {
    int broj_sekcija = lista_sekcija.size();
    
    for (iter_relZapis = lista_relZapisa.begin(); iter_relZapis != lista_relZapisa.end(); iter_relZapis++) {
        if ((*iter_relZapis)->vidljivost_simbola == 'g') {
            (*iter_relZapis)->red_br_simbola = (*iter_relZapis)->red_br_simbola + (broj_sekcija - 1);
        }    
    }
}

void ispis_izlaza_u_fajl() {
   int i = 0;
   while(i++ < 100) izlaz << "*";

   izlaz << "\n" << setw(55);
   izlaz << "Tabela simbola" << endl;

    i = 0;
    while(i++ < 100)  izlaz << "-";

    izlaz << "\n" << setw(15) << "Naziv" << setw(15) << "Sekcija" << setw(15) << "Vrednost" << setw(15) << "Vidljivost" << setw(15) << "Redni broj" << setw(15) << "Velicina" << endl;

    i = 0;
    while(i++ < 100)  izlaz << "-";
    izlaz << "\n";

    // ispis tabele simbola
    for (iter_simbol = tabela_simbola.begin(); iter_simbol != tabela_simbola.end(); iter_simbol++)   {
        izlaz << setw(14) << (*iter_simbol)->naziv << setw(13) << (*iter_simbol)->sekcija << setw(15) << std::hex << (*iter_simbol)->vrednost << setw(13) << (*iter_simbol)->vidljivost << setw(15) << std::dec << (*iter_simbol)->redniBroj << setw(16) << std::hex << (*iter_simbol)->velicina << endl;
    }

    i = 0;
    while(i++ < 100) izlaz << "*";
    izlaz << "\n";
    
    // ispis realokacionih zapisa
    for (iter_simbol = lista_sekcija.begin(); iter_simbol != lista_sekcija.end(); iter_simbol++) {
        if (/*(*iter_simbol)->naziv != ".bss" && */(*iter_simbol)->naziv != "UND") {
           int brZapisa = dohvati_brRelZapisa_za_sekciju((*iter_simbol)->sekcija);
           if (brZapisa > 0) {
                
                izlaz << "#.rel" <<  (*iter_simbol)->naziv << endl;
                izlaz << "Offset" << setw(15) << "Tip" << setw(15) << "Simbol" << setw(15) << endl;
                for (iter_relZapis = lista_relZapisa.begin(); iter_relZapis != lista_relZapisa.end(); iter_relZapis++) {
                    if ((*iter_relZapis)->red_br_sekcije == (*iter_simbol)->sekcija) {
                    izlaz << setw(4) << std::hex << (*iter_relZapis)->offset << setw(19) << (*iter_relZapis)->tip << setw(10) << std::dec << (*iter_relZapis)->red_br_simbola << endl;
                    }
                }
           }
        }
        // izlaz << "\n";
    }

    i = 0;
    while(i++ < 100) izlaz << "*";
    izlaz << "\n";

    // ispis masinskog koda
    for (iter_simbol = lista_sekcija.begin(); iter_simbol != lista_sekcija.end(); iter_simbol++) {
        if (/*(*iter_simbol)->naziv != ".bss" && */(*iter_simbol)->naziv != "UND") {
            izlaz << "#" <<  (*iter_simbol)->naziv;

            int red_br_sekcije = (*iter_simbol)->redniBroj;
            string mas_kod = dodavanje_razmaka(sadrzajZaIspis[red_br_sekcije - 1].masinskiKod);
            for (unsigned int i = 0; i < mas_kod.size(); i++) {
                if ((i % 99) == 0) {
                    izlaz << "\n";
                }
                izlaz << mas_kod[i];
            }
            izlaz << "\n";
        }
        // izlaz << "\n";
    }

}

string dodavanje_razmaka(string str) {
    string tmp = "";
    for (unsigned int i = 0; i < str.size(); i++) {
        tmp += str[i];
        if ((i % 2) != 0) {
            tmp += " ";
        }
    }
    return tmp;
}


int dohvati_brRelZapisa_za_sekciju(int redBrSek) {
    int cnt = 0;
    for (iter_relZapis = lista_relZapisa.begin(); iter_relZapis != lista_relZapisa.end(); iter_relZapis++) {
        if ((*iter_relZapis)->red_br_sekcije == redBrSek) {
            ++cnt;
        }
    }

    return cnt;
}

