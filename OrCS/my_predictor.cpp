
#include "simulator.hpp"

my_predictor_t::my_predictor_t() {
    this->global_bhr = 0;
    this->local_bht = (uint64_t*) calloc(sizeof(uint64_t), 1023);
    this->local_pht = (uint64_t*) calloc(sizeof(uint64_t), 1023);
    this->global_pht =  (uint64_t*) calloc(sizeof(uint64_t), 1023);
    this->choice_pht =  (uint64_t*) calloc(sizeof(uint64_t), 1023);
};

void my_predictor_t::see_the_future(uint64_t PC, uint8_t outcome) {

    uint64_t prediction;
    uint64_t pcbits;
    uint64_t hybridbits;
    uint64_t XOR;
    uint64_t choice;
    uint64_t local_hist;

    pcbits = PC & 1023;
    hybridbits = global_bhr & 511;
    (hybridbits<<9);
    hybridbits = hybridbits | (512 & local_bht[pcbits]);
    XOR = hybridbits ^ pcbits;
    choice = choice_pht[XOR];

    if(choice<2) {
        local_hist = 1023 & local_bht[pcbits];
        prediction = local_pht[local_hist];
    } else {
        prediction = global_pht[XOR];
    }

    if(prediction>1) {
        if (outcome == 0)  {
            orcs_engine.taken += 1;
            orcs_engine.predicted += 1;
        } else {
            orcs_engine.not_taken += 1;
            orcs_engine.not_predicted += 1;
        }
    } else {
        if (outcome == 1) {
            orcs_engine.not_taken += 1;
            orcs_engine.predicted += 1;
        } else {
            orcs_engine.not_predicted += 1;
            orcs_engine.taken += 1;
        } 
    }
};

void my_predictor_t::update_predictor(uint64_t PC, uint8_t outcome) {
  
    uint64_t pcbits;
    uint64_t hybridbits;
    uint64_t XOR;
    uint64_t local_hist;
    uint64_t local_predction;
    uint64_t global_prediction;

    pcbits = PC & 1023;
    hybridbits = global_bhr & 511;
    
    (hybridbits<<9);
    hybridbits = hybridbits | (512 & local_bht[pcbits]);

    XOR = hybridbits ^ pcbits;
    local_hist = 1023 & local_bht[pcbits];

    local_predction = local_pht[local_hist];
    if(local_predction>1)
        local_predction = 0;
    else
        local_predction = 1;

    global_prediction = global_pht[XOR];
    if(global_prediction>1)
        global_prediction = 0;
    else
        global_prediction = 1;

    if(global_prediction == outcome && local_predction != outcome && choice_pht[XOR] != 3)
        choice_pht[XOR] += 1;
    else if(global_prediction != outcome && local_predction == outcome && choice_pht[XOR] != 0)
        choice_pht[XOR] -= 1;

    if(outcome==0) {
        if(global_pht[XOR] !=3)
            global_pht[XOR] += 1;
        if(local_pht[local_hist] !=3)
            local_pht[local_hist] += 1;
    } else {
        if(global_pht[XOR] != 0)
            global_pht[XOR] -= 1;
        if(local_pht[local_hist] != 0)
            local_pht[local_hist] -= 1;
    }

    local_bht[pcbits] = local_bht[pcbits]<<1 | outcome;
    global_bhr = global_bhr<<1 | outcome;
};