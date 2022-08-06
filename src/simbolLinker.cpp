#include "../inc/simbolLinker.h"


SimbolLinker::SimbolLinker(string n, int sek, int vr, char vid, int rb, int vel, int red_br_fajla, bool obr) {
    this->naziv = n;
    this->sekcija = sek;
    this->vrednost = vr;
    this->vidljivost = vid;
    this->redniBroj = rb;
    this->velicina = vel;
    this->red_br_fajla = red_br_fajla;
    this->obradjen = obr;
}