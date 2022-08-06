#include "../inc/relZapisLinker.h"

RelZapisLinker::RelZapisLinker(int offset, string tip, int rbSimbola, string nazivSekcije, int rbFajla, int obr) {
    this->offset = offset;
    this->tip = tip;
    this->red_br_simbola = rbSimbola;
    this->nazivSekcije = nazivSekcije;
    this->red_br_fajla = rbFajla;
    this->obradjen = obr;
}