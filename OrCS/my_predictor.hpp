#define TAKEN 0
#define NOTAKEN 1

class my_predictor_t {
    private:

    public:
        // ====================================================================
		/// Attributes
		// ====================================================================
        uint64_t global_bhr = 0;
        uint64_t *local_bht;
        uint64_t *local_pht;
        uint64_t *global_pht;
        uint64_t *choice_pht;
  

        // ====================================================================
		/// Methods
		// ====================================================================
        my_predictor_t();
        void see_the_future(uint64_t PC, uint8_t outcome);
        void update_predictor(uint64_t PC, uint8_t outcome);
};
