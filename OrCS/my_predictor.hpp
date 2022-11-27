
class my_predictor_t {
    private:

    public:
        // ====================================================================
		/// Attributes
		// ====================================================================
        uint64_t gbhr = 0;
        uint64_t *local_bht = (uint64_t) calloc(sizeof(uint64_t), 1023);
        uint64_t *local_pht = (uint64_t) calloc(sizeof(uint64_t), 1023);
        uint64_t *global_pht =  (uint64_t) calloc(sizeof(uint64_t), 1023);
        uint64_t *choice_pht =  (uint64_t) calloc(sizeof(uint64_t), 1023);
  

        // ====================================================================
		/// Methods
		// ====================================================================
        void see_the_future(uint64_t pc);
        void update_predictor(uint64_t pc, uint8_t outcome);
};
