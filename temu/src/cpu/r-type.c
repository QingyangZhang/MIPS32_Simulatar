#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* decode R-type instrucion */
static void decode_r_type(uint32_t instr) {

	op_src1->type = OP_TYPE_REG;
	op_src1->reg = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	op_src1->val = reg_w(op_src1->reg);
	
	op_src2->type = OP_TYPE_REG;
	op_src2->reg = (instr & RT_MASK) >> (RD_SIZE + SHAMT_SIZE + FUNC_SIZE);
	op_src2->val = reg_w(op_src2->reg);

	op_dest->type = OP_TYPE_REG;
	op_dest->reg = (instr & RD_MASK) >> (SHAMT_SIZE + FUNC_SIZE);
}

make_helper(and) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val & op_src2->val);
	sprintf(assembly, "and   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(add) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = ((int32_t)op_src1->val + (int32_t)op_src2->val);
	sprintf(assembly, "add   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(addu) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val + op_src2->val);
	sprintf(assembly, "addu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sub) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = ((int32_t)op_src1->val - (int32_t)op_src2->val);
	sprintf(assembly, "sub   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(subu) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val - op_src2->val);
	sprintf(assembly, "subu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(slt) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (int32_t)op_src1->val < (int32_t)op_src2->val ? 1 : 0;
	sprintf(assembly, "slt   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sltu) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = op_src1->val < op_src2->val ? 1 : 0;
	sprintf(assembly, "sltu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(div) {

	decode_r_type(instr);
	reg_w(cpu.hi) = (int32_t)op_src1->val % (int32_t)op_src2->val;
	reg_w(cpu.lo) = (int32_t)op_src1->val / (int32_t)op_src2->val;
	sprintf(assembly, "div   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(divu) {

	decode_r_type(instr);
	reg_w(cpu.hi) = op_src1->val % op_src2->val;
	reg_w(cpu.lo) = op_src1->val / op_src2->val;
	sprintf(assembly, "divu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(mult) {

	decode_r_type(instr);
	int64_t result = ((int64_t)(int32_t)op_src1->val) * ((int64_t)(int32_t)op_src2->val);
	//printf("op1:%d,op2:%d\n",(int32_t)op_src1->val,(int32_t)op_src2->val);
	cpu.lo = result & 0xffffffff;
	cpu.hi = result>>32;
	//printf("result:%08lx\n",result);
	//printf("mult: hi=%x,lo=%x\n",cpu.hi,cpu.lo);
	sprintf(assembly, "mult   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(multu) {

	decode_r_type(instr);
	uint64_t result = op_src1->val * op_src2->val;
	cpu.lo = (uint32_t)result;
	cpu.hi = (uint32_t)(result>>32);
	//printf("op1:%x,op2:%x\n",op_src1->val,op_src2->val);
	//printf("result:%08lx\n",result);
	//printf("mult: hi=%x,lo=%x\n",cpu.hi,cpu.lo);
	sprintf(assembly, "multu   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(nor) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = !(op_src1->val | op_src2->val);
	sprintf(assembly, "nor   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(or) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val | op_src2->val);
	sprintf(assembly, "or   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(xor) {

	decode_r_type(instr);
	reg_w(op_dest->reg) = (op_src1->val ^ op_src2->val);
	sprintf(assembly, "xor   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sllv) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  op_src2->val << op_src1->val;
	sprintf(assembly, "sllv   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sll) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  op_src2->val << ((instr & SHAMT_MASK)>>FUNC_SIZE);
	sprintf(assembly, "sll   %s,   %s,   0x%02x", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), (instr & SHAMT_MASK));
}

make_helper(srav) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  (int32_t)op_src2->val >> op_src1->val;
	sprintf(assembly, "srav   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(sra) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  (int32_t)op_src2->val >> ((instr & SHAMT_MASK)>>FUNC_SIZE);
	sprintf(assembly, "sra   %s,   %s,   0x%02x", REG_NAME(op_dest->reg), REG_NAME(op_src2->reg), (instr & SHAMT_MASK));
}

make_helper(srlv) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  op_src2->val >> op_src1->val;
	sprintf(assembly, "srlv   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(srl) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  op_src2->val >> ((instr & SHAMT_MASK)>>FUNC_SIZE);
	sprintf(assembly, "srl   %s,   %s,   %s", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), REG_NAME(op_src2->reg));
}

make_helper(mfhi) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  cpu.hi;
	sprintf(assembly, "mfhi   %s   ", REG_NAME(op_dest->reg));
}

make_helper(mflo) {

	decode_r_type(instr);
	reg_w(op_dest->reg) =  cpu.lo;
	sprintf(assembly, "mflo   %s   ", REG_NAME(op_dest->reg));
}

make_helper(mthi) {

	decode_r_type(instr);
	cpu.hi = op_src1->val;
	sprintf(assembly, "mthi   %s   ", REG_NAME(op_src1->reg));
}

make_helper(mtlo) {

	decode_r_type(instr);
	cpu.lo = op_src1->val;
	sprintf(assembly, "mtlo   %s   ", REG_NAME(op_src1->reg));
}

