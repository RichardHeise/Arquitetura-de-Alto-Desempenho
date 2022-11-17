#include "simulator.hpp"

btb_cell_t::btb_cell_t() {
    this->opcode_address = 0;
    this->access_cycle = 0;
    this->target_address = 0;
    this->bits_predictor = 0;
};

