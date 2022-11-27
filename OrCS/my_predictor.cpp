
#include "simulator.hpp"

void my_predictor_t::init_predictor() {

    gbhr = 0;

    // Local BHT
    local_bht = (uint32_t*) calloc(sizeof(uint32_t), 1023);

    // Local PHT
    local_pht = (uint32_t*) calloc(sizeof(uint32_t), 1023);

    // Global PHT
    global_pht = (uint32_t*) calloc(sizeof(uint32_t), 1023);

    // Choice PHT
    choice_pht = (uint32_t*) calloc(sizeof(uint32_t), 1023);
  
}

void my_predictor_t::see_the_future(uint64_t pc) {

    uint32_t prediction;
    uint32_t pcbits;
    uint32_t histbits;
    uint32_t index;
    uint32_t choice;
    uint32_t pcidx;
    uint32_t lhist;

    pcbits = pc & 1023;
    histbits = gbhr & 1023;
    index = histbits ^ pcbits;
    choice = choice_pht[index];

    if(choice<2) {
        pcidx = 1023 & pc;
        lhist = 1023 & local_bht[pcidx];
        prediction = local_pht[lhist];
    } else {
        prediction = global_pht[index];
    }

    if(prediction>1)
        orcs_engine.taken += 1;
    else
        orcs_engine.not_taken += 1;


    orcs_engine.not_taken += 1;
}


void my_predictor_t::update_predictor(uint64_t pc, uint8_t outcome) {
  
    uint32_t pcbits;
    uint32_t histbits;
    uint32_t index;
    uint32_t pcidx;
    uint32_t lhist;
    uint32_t lpred;
    uint32_t gpred;

    pcbits = pc & 1023;
    histbits = gbhr & 1023;
    index = histbits ^ pcbits;
    pcidx = 1023 & pc;
    lhist = 1023 & local_bht[pcidx];

    lpred = local_pht[lhist];
    if(lpred>1)
        lpred = orcs_engine.taken;
    else
        lpred = orcs_engine.not_taken;

    gpred = global_pht[index];
    if(gpred>1)
        gpred = orcs_engine.taken;
    else
        gpred = orcs_engine.not_taken;

    if(gpred==outcome && lpred!=outcome && choice_pht[index]!=3)
        choice_pht[index]++;
    else if(gpred!=outcome && lpred==outcome && choice_pht[index]!=0)
        choice_pht[index]--;

    if(outcome==orcs_engine.taken) {
        if(global_pht[index]!=3)
            global_pht[index]++;
        if(local_pht[lhist]!=3)
            local_pht[lhist]++;
    } else {
        if(global_pht[index]!=0)
            global_pht[index]--;
        if(local_pht[lhist]!=0)
            local_pht[lhist]--;
    }

    local_bht[pcidx] = local_bht[pcidx]<<1 | outcome;
    gbhr = gbhr<<1 | outcome;
    return;
}