#include "../inc/simbol.h"

Simbol::Simbol() {
    this->naziv = "";
    this->sekcija = -1;
    this->vrednost = -1;
    this->vidljivost = ' ';
    this->redniBroj = 0;
    this->velicina = 0;
    this->definisan = false;
}

Simbol::Simbol(string n, int sek, int vr, char vid, int rb, int vel, bool def, bool extrn) {
    this->naziv = n;
    this->sekcija = sek;
    this->vrednost = vr;
    this->vidljivost = vid;
    this->redniBroj = rb;
    this->velicina = vel;
    this->definisan = def;
    this->extrn = extrn;
}

void Simbol::dodaj(Flink* zapis) {
    lista.push_back(zapis);
}