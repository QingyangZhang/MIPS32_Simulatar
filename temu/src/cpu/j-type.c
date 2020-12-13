#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* decode J-type instrucion */

make_helper(j) {
	uint32_t instr_index=(instr&0x3FFFFFF);
	cpu.pc = ((cpu.pc>>28)<<28)|(instr_index<<2);
	sprintf(assembly, "j   0x%08x", instr_index);
}

make_helper(jal){
	reg_w(31) = cpu.pc+8;
	uint32_t instr_index=(instr&0x3FFFFFF);
	cpu.pc = ((cpu.pc>>28)<<28)|(instr_index<<2);
	sprintf(assembly, "jal   0x%08x", instr_index);
}

make_helper(jr){
	uint32_t rs = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	uint32_t temp = reg_w(rs);
	cpu.pc = temp;
	sprintf(assembly, "jr");
}

make_helper(jalr){
	uint32_t rs = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	uint32_t rd = (instr & RD_MASK) >> (SHAMT_SIZE + FUNC_SIZE);
	uint32_t temp = reg_w(rs);
	reg_w(rd) = cpu.pc + 8;
	cpu.pc = temp;
	sprintf(assembly, "jalr");
}
