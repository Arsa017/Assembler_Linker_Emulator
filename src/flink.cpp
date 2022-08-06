#include "../inc/flink.h"

Flink::Flink() {
    this->pozicija = 0;
    this->operacija = -1;
    this->sekcija = -1;
    this->br_bajtova = -1;
}

Flink::Flink(int poz, int sek, int operac, int brBajt) {
    this->pozicija = poz;
    this->sekcija = sek;
    this->operacija = operac;
    this->br_bajtova = brBajt;
}

