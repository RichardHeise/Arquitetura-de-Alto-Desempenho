#include "simulator.hpp"

// =====================================================================
processor_t::processor_t() {

};

// =====================================================================
void processor_t::allocate() {

};

// =====================================================================
void processor_t::clock() {

	// Penalty if miss prediction happens
	if (orcs_engine.miss_predicted != 0) {
		orcs_engine.miss_predicted -= 1;
		return;
	} 

	opcode_package_t instruction;
	static opcode_package_t next_instruction;

	instruction = next_instruction;

	/// Get the next instruction from the trace
	if (!orcs_engine.trace_reader->trace_fetch(&next_instruction)) {
		/// If EOF
		orcs_engine.simulator_alive = false;
	}

	predictionHandler(instruction, next_instruction);
};

// =====================================================================
void processor_t::statistics() {
	printf("hits: %lu\n", orcs_engine.hits);
    printf("misses: %lu\n", orcs_engine.misses);
    printf("taken: %lu\n", orcs_engine.taken);
    printf("not_taken: %lu\n", orcs_engine.not_taken);
	printf("predicted: %lu\n", orcs_engine.predicted);
	printf("miss_predicted: %lu\n", orcs_engine.miss_predicted);
	printf("miss_predicted percentage: %lu\n",  orcs_engine.predicted*100 / (orcs_engine.miss_predicted + orcs_engine.predicted));
	ORCS_PRINTF("######################################################\n");
	ORCS_PRINTF("processor_t\n");

};

// =====================================================================
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

// =====================================================================
void processor_t::condHandler(opcode_package_t instruction, opcode_package_t next_instruction, int tag, int i) {
	if ( instruction.opcode_address + instruction.opcode_size == next_instruction.opcode_address ) {
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
};

// =====================================================================
void processor_t::predictionHandler(opcode_package_t instruction, opcode_package_t next_instruction) {

	if (instruction.opcode_operation == INSTRUCTION_OPERATION_BRANCH) {

		int tag = instruction.opcode_address & 1023;

		for ( int i = 0; i < 4; i++ ) {

			if ( BTB[tag][i].opcode_address == instruction.opcode_address ) {
				orcs_engine.hits += 1;

				BTB[tag][i].target_address = next_instruction.opcode_address;
				BTB[tag][i].access_cycle = orcs_engine.global_cycle;
				if (instruction.branch_type == BRANCH_COND) {
					condHandler(instruction, next_instruction, tag, i);
				}

				return;
			}
		}

		int lesser = 0;
		orcs_engine.misses += 1;
		for (int i = 0; i < 4; i++) {
			if (BTB[tag][i].opcode_address == 0) {
				BTB[tag][i].opcode_address = instruction.opcode_address;
				BTB[tag][i].access_cycle = orcs_engine.global_cycle;
				BTB[tag][i].target_address = next_instruction.opcode_address;
				BTB[tag][i].bits_predictor = 0;
				return;
			} else if (BTB[tag][i].access_cycle < BTB[tag][lesser].access_cycle) {
				lesser = i;
			}
		}

		BTB[tag][lesser].opcode_address = instruction.opcode_address;
		BTB[tag][lesser].access_cycle = orcs_engine.global_cycle;
		BTB[tag][lesser].target_address = next_instruction.opcode_address;
		BTB[tag][lesser].bits_predictor = 0;
		
	}

};