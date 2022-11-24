#include "simulator.hpp"

// =====================================================================
processor_t::processor_t() {

};

// =====================================================================
void processor_t::allocate() {

};

// =====================================================================
void processor_t::clock() {

	if (orcs_engine.miss_predicted != 0) {
		orcs_engine.miss_predicted -= 1;
		return;
	}

	/// Get the next instruction from the trace
	static opcode_package_t new_instruction;
	opcode_package_t old_instruction;

	old_instruction = new_instruction;

	if (!orcs_engine.trace_reader->trace_fetch(&new_instruction)) {
		/// If EOF
		orcs_engine.simulator_alive = false;
	}

	if (old_instruction.opcode_operation == INSTRUCTION_OPERATION_BRANCH) {

		int tag = old_instruction.opcode_address & 1023;

		for ( int i = 0; i < 4; i++ ) {

			if ( BTB[tag][i].opcode_address == old_instruction.opcode_address ) {
				orcs_engine.hits += 1;
				BTB[tag][i].target_address = new_instruction.opcode_address;

				BTB[tag][i].access_cycle = orcs_engine.global_cycle;
				if (old_instruction.branch_type == BRANCH_COND) {
					if ( old_instruction.opcode_address + old_instruction.opcode_size == new_instruction.opcode_address ) {
						if ( BTB[tag][i].bits_predictor < 2 ) {
							orcs_engine.predicted += 1;
						} else {
							orcs_engine.miss_predicted = 16;
							orcs_engine.not_predicted += 1;
						}

						if ( BTB[tag][i].bits_predictor > 0) {
							BTB[tag][i].bits_predictor -= 1;
						}

						orcs_engine.not_taken += 1;

					} else {
						if ( BTB[tag][i].bits_predictor > 1 ) {
							orcs_engine.predicted += 1;
						} else {
							orcs_engine.miss_predicted = 16;
							orcs_engine.not_predicted += 1;
						}

						if ( BTB[tag][i].bits_predictor < 3) {
							BTB[tag][i].bits_predictor += 1;
						}

						orcs_engine.taken += 1;
					}
				}

				return;
			}
		}

		int lesser = 0;
		orcs_engine.misses += 1;
		for (int i = 0; i < 4; i++) {
			if (BTB[tag][i].opcode_address == 0) {
				BTB[tag][i].opcode_address = old_instruction.opcode_address;
				BTB[tag][i].access_cycle = orcs_engine.global_cycle;
				BTB[tag][i].target_address = new_instruction.opcode_address;
				BTB[tag][i].bits_predictor = 0;
				return;
			} else if (BTB[tag][i].access_cycle < BTB[tag][lesser].access_cycle) {
				lesser = i;
			}
		}

		BTB[tag][lesser].opcode_address = old_instruction.opcode_address;
		BTB[tag][lesser].access_cycle = orcs_engine.global_cycle;
		BTB[tag][lesser].target_address = new_instruction.opcode_address;
		BTB[tag][lesser].bits_predictor = 0;
		
	}

};

// =====================================================================
void processor_t::statistics() {
	ORCS_PRINTF("######################################################\n");
	ORCS_PRINTF("processor_t\n");

};

void processor_t::print_trace(opcode_package_t new_instruction){
	printf("opcode_operation: %d\n",new_instruction.opcode_operation);
	printf("opcode_address: %ld\n",new_instruction.opcode_address);
	printf("opcode_size: %d\n",new_instruction.opcode_size);
	for (int i = 0; i < 16; i++){
		if(new_instruction.read_regs[i]!=0){
			printf("read_reg %d: %d\n",i,new_instruction.read_regs[i]);
		}
	}
	for (int i = 0; i < 16; i++)
	{
		if(new_instruction.write_regs[i]!=0){
			printf("write_reg %d: %d\n",i,new_instruction.write_regs[0]);
		}
	}
	
	printf("base_reg: %d\n",new_instruction.base_reg);
	printf("index_reg: %d\n",new_instruction.index_reg);
	printf("is_read: %d\n",new_instruction.is_read);
	if(new_instruction.is_read){
		printf("read_addres: %ld\n",new_instruction.read_address);
		printf("read_size: %d\n",new_instruction.read_size);
	}
	printf("is_read2: %d\n",new_instruction.is_read2);
	if(new_instruction.is_read2){
		printf("read2_address: %ld\n",new_instruction.read2_address);
		printf("read2_size: %d\n",new_instruction.read2_size);
	}
	printf("is_write: %d\n",new_instruction.is_write);
	if(new_instruction.is_write){
		printf("write_address: %ld\n",new_instruction.write_address);
		printf("write_size: %d\n",new_instruction.write_size);
	}
	printf("branch_type: %d\n",new_instruction.branch_type);
	printf("is_indirect: %d\n",new_instruction.is_indirect);
	printf("is_predicated: %d\n",new_instruction.is_predicated);
	printf("is_prefetch: %d\n",new_instruction.is_prefetch);
	printf("--------------------------\n");

};