
class my_predictor_t {
    private:

    public:
        // ====================================================================
		/// Attributes
		// ====================================================================
        int ghistoryBits; // Number of bits used for Global History
        int lhistoryBits; // Number of bits used for Local History
        int pcIndexBits;  // Number of bits used for PC index
        uint32_t gbhr;
        uint32_t *local_bht;
        uint32_t *local_pht;
        uint32_t *global_pht;
        uint32_t *choice_pht;

        // ====================================================================
		/// Methods
		// ====================================================================
        void init_predictor();
        void see_the_future(uint64_t pc);
        void update_predictor(uint64_t pc, uint8_t outcome);
};
