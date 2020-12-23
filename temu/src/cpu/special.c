#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* invalid opcode */
make_helper(inv) {

	uint32_t temp;
	temp = instr_fetch(pc, 4);

	uint8_t *p = (void *)&temp;
	printf("invalid opcode(pc = 0x%08x): %02x %02x %02x %02x ...\n\n", 
			pc, p[3], p[2], p[1], p[0]);

	printf("There are two cases which will trigger this unexpected exception:\n\
1. The instruction at pc = 0x%08x is not implemented.\n\
2. Something is implemented incorrectly.\n", pc);
	printf("Find this pc value(0x%08x) in the disassembling result to distinguish which case it is.\n\n", pc);

	assert(0);
}

/* stop temu */
make_helper(temu_trap) {

	printf("\33[1;31mtemu: HIT GOOD TRAP\33[0m at $pc = 0x%08x\n\n", cpu.pc);

	temu_state = END;

}

make_helper(eret) {
	cpu.pc = cp0.epc;
	sprintf(assembly, "epc");
}

make_helper(mfc0) {
	uint32_t rt = (instr&((31)<<21));
	uint32_t rd = (instr&((31)<<11));
	reg_w(rt) = cp0.gpr[rd]._32;
}

make_helper(mtc0) {
	uint32_t rt = (instr&((31)<<21));
	uint32_t rd = (instr&((31)<<11));
	cp0.gpr[rd]._32 = reg_w(rt);
}



