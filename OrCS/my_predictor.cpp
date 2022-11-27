
#include "simulator.hpp"

void my_predictor_t::see_the_future(uint64_t pc) {

    uint64_t prediction;
    uint64_t pcbits;
    uint64_t histbits;
    uint64_t index;
    uint64_t choice;
    uint64_t pcidx;
    uint64_t lhist;

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
}


void my_predictor_t::update_predictor(uint64_t pc, uint8_t outcome) {
  
    uint64_t pcbits;
    uint64_t histbits;
    uint64_t index;
    uint64_t pcidx;
    uint64_t lhist;
    uint64_t lpred;
    uint64_t gpred;

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
}