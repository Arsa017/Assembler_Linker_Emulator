#include "../inc/relZapis.h"

RelZapis::RelZapis() {
    this->offset = 0;
    this->tip = "";
    this->red_br_simbola = 0;
    this->red_br_sekcije = 0;
}

RelZapis::RelZapis(int offset, string tip, int rbSimbola, int rbSekcije, char vidljivost_simbola) {
    this->offset = offset;
    this->tip = tip;
    this->red_br_simbola = rbSimbola;
    this->red_br_sekcije = rbSekcije;
    this->vidljivost_simbola = vidljivost_simbola;
}