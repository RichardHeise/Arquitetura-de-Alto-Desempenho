// Richard Fernando Heise Ferreira (GRR20191053)
/// This is a mixture of lgshare + alpha 21264 
#include "simulator.hpp"

// Initializing instance of object
my_predictor_t::my_predictor_t() {
    this->global_bhr = 0;
    this->local_bht = (uint64_t*) calloc(sizeof(uint64_t), 1023);
    this->local_pht = (uint64_t*) calloc(sizeof(uint64_t), 1023);
    this->global_pht =  (uint64_t*) calloc(sizeof(uint64_t), 1023);
    this->choice_pht =  (uint64_t*) calloc(sizeof(uint64_t), 1023);
};

// try to predict the branch
void my_predictor_t::see_the_future(uint64_t PC, uint8_t outcome) {

    uint64_t prediction; // prediction made by predictor
    uint64_t pcbits;     // bits taken from PC
    uint64_t hybridbits; // bits from the hybrid register
    uint64_t XOR;        // the XOR indexing the choice pht 
    uint64_t choice;     // a choice from the choice pht
    uint64_t local_hist; // bits from local bht

    pcbits = PC & 1023; // get last 10 bits from PC
    hybridbits = global_bhr & 511; // get last 5 bits from global bhr

    (hybridbits<<9); // shift register to open space
    hybridbits = hybridbits | (512 & local_bht[pcbits]); // concatenate pc + local bht bits

    XOR = hybridbits ^ pcbits; // XOR-ing register with pc
    choice = choice_pht[XOR];  // getting a choice from the choice pht

    // if choice is > 2 then we use global pht, else we use local bht
    if(choice < 2) {
        local_hist = 1023 & local_bht[pcbits]; // indexing local pht
        prediction = local_pht[local_hist]; // taken from local pht
    } else {
        prediction = global_pht[XOR]; // if choice was TAKEN we use global pht
    }

    // if prediction is TAKEN...
    if(prediction > 1) {
        // AND the outcome was TAKEN, we did it!
        if (outcome == TAKEN)  {
            orcs_engine.taken += 1;
            orcs_engine.predicted += 1;
        } else {
            // AND the outcome was NOT TAKEN, we failed it!
            orcs_engine.not_taken += 1;
            orcs_engine.not_predicted += 1;
            orcs_engine.miss_predicted = 16; // penalty
        }
    // if prediction is NOT TAKEN...
    } else {
        // AND the outcome was NOT TAKEN, we did it!
        if (outcome == NOTAKEN) {
            orcs_engine.not_taken += 1;
            orcs_engine.predicted += 1;
        } else {
            // AND the outcome was TAKEN, we failed it!
            orcs_engine.not_predicted += 1;
            orcs_engine.taken += 1;
            orcs_engine.miss_predicted = 16;
        } 
    }
};

void my_predictor_t::update_predictor(uint64_t PC, uint8_t outcome) {
  
    uint64_t pcbits;            // bits taken from PC
    uint64_t hybridbits;        // bits from hybrid register
    uint64_t XOR;               // XOR for indexing
    uint64_t local_hist;        // bits from local bht
    uint64_t local_prediction;  // prediction from local pht
    uint64_t global_prediction; // prediction from global pht

    pcbits = PC & 1023; // last 10 bits from PC
    hybridbits = global_bhr & 511; // last 5 bits from global bhr
    
    (hybridbits<<9); // opening space for local bht 
    hybridbits = hybridbits | (512 & local_bht[pcbits]); // concatening global bhr + local pht bits

    XOR = hybridbits ^ pcbits; // XOR-ing hybrid register and pcbits
    local_hist = 1023 & local_bht[pcbits]; // index for local pht

    local_prediction = local_pht[local_hist]; // saving local pht

    // if local predictor is TAKEN
    if(local_prediction > 1)
        local_prediction = TAKEN; 
    else
        local_prediction = NOTAKEN;

    global_prediction = global_pht[XOR]; // saving global pht prediction

    // if global prediction is TAKEN
    if (global_prediction > 1)
        global_prediction = TAKEN;
    else
        global_prediction = NOTAKEN;

    // if global is equal to outcome and local is not we trust global 
    if (global_prediction == outcome && local_prediction != outcome && choice_pht[XOR] != 3) {
        choice_pht[XOR] += 1;

    // if global is not equal to outcome and local is equal to outcome we trust local
    } else if (global_prediction != outcome && local_prediction == outcome && choice_pht[XOR] != 0) {
        choice_pht[XOR] -= 1;
    }

    // if outcome is TAKEN...
    if (outcome == TAKEN) {
        // AND we can increment global pht, we do
        if (global_pht[XOR] != 3)
            global_pht[XOR] += 1;
        // AND we can increment local pht, we do
        if (local_pht[local_hist] != 3)
            local_pht[local_hist] += 1;
    } else {
        // if outcome is NOT TAKEN...
        // AND we can decrement global pht, we do
        if (global_pht[XOR] != 0)
            global_pht[XOR] -= 1;
        // AND we can decrement local pht, we do
        if (local_pht[local_hist] != 0)
            local_pht[local_hist] -= 1;
    }

    // adding outcome to local bht and global bht
    local_bht[pcbits] = (local_bht[pcbits]<<1) | outcome;
    global_bhr = (global_bhr<<1) | outcome;
};