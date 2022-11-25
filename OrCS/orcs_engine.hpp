// ============================================================================
class orcs_engine_t {
	private:


    public:
        /// Program input
        char *arg_trace_file_name;

        /// Control the Global Cycle
        uint64_t global_cycle;

        bool simulator_alive;

        uint64_t hits = 0;
        uint64_t misses = 0;
        uint64_t taken = 0;
        uint64_t not_taken = 0;
        uint64_t predicted = 0;
        uint64_t not_predicted = 0;
        uint64_t miss_predicted = 0;

        /// Components modeled
        trace_reader_t *trace_reader;
        processor_t *processor;

		// ====================================================================
		/// Methods
		// ====================================================================
		orcs_engine_t();
		void allocate();
        uint64_t get_global_cycle() {
            return this->global_cycle;
        };
};
