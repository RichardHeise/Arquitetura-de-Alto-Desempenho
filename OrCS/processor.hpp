// ============================================================================
// ============================================================================
class processor_t {
    private:    
    
    
    public:

		// ====================================================================
		/// Attributes
		// ====================================================================
		btb_cell_t BTB[1024][4];
		my_predictor_t predictor;

		// ====================================================================
		/// Methods
		// ====================================================================
		processor_t();
	    void allocate();
	    void clock();
	    void statistics();
		void print_trace(opcode_package_t new_instruction);
		void two_bits(opcode_package_t instruction, opcode_package_t new_instruction, int tag, int i);
		void BTBhandler(opcode_package_t instruction, opcode_package_t next_instruction, bool default_handler);
		void other_predictor(opcode_package_t instruction, opcode_package_t next_instruction);
};
