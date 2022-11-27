// ============================================================================
// ============================================================================
class processor_t {
    private:    
    
    
    public:

		// ====================================================================
		/// Attributes
		// ====================================================================
		btb_cell_t BTB[1024][4];

		// ====================================================================
		/// Methods
		// ====================================================================
		processor_t();
	    void allocate();
	    void clock();
	    void statistics();
		void print_trace(opcode_package_t new_instruction);
		void condHandler(opcode_package_t instruction, opcode_package_t new_instruction, int tag, int i);
		void predictionHandler(opcode_package_t instruction, opcode_package_t next_instruction, bool TAGE);
		void two_bits(opcode_package_t instruction, opcode_package_t next_instruction);
		void other_predictor(opcode_package_t instruction, opcode_package_t next_instruction);
};
