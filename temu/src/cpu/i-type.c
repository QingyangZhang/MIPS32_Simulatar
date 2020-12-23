#include "helper.h"
#include "monitor.h"
#include "reg.h"

extern uint32_t instr;
extern char assembly[80];

/* decode I-type instrucion with unsigned immediate */
static void decode_imm_type(uint32_t instr) {

	op_src1->type = OP_TYPE_REG;
	op_src1->reg = (instr & RS_MASK) >> (RT_SIZE + IMM_SIZE);
	op_src1->val = reg_w(op_src1->reg);
	
	op_src2->type = OP_TYPE_IMM;
	op_src2->imm = instr & IMM_MASK;
	op_src2->val = op_src2->imm;

	op_dest->type = OP_TYPE_REG;
	op_dest->reg = (instr & RT_MASK) >> (IMM_SIZE);
	op_dest->val = reg_w(op_dest->reg);
}

make_helper(lui) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = (op_src2->val << 16);
	sprintf(assembly, "lui   %s,   0x%04x", REG_NAME(op_dest->reg), op_src2->imm);
}

make_helper(addi) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val + op_src2->val;
	sprintf(assembly, "addi   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(addiu) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val + ((((int32_t)op_src2->val)<<16)>>16);
	sprintf(assembly, "addiu   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(slti) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = (int32_t)op_src1->val < ((((int32_t)op_src2->val)<<16)>>16) ? 1 : 0;
	sprintf(assembly, "slti   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(sltiu) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val < ((((int32_t)op_src2->val)<<16)>>16) ? 1 : 0;
	sprintf(assembly, "sltiu   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(andi) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val & (uint32_t)op_src2->val;
	sprintf(assembly, "andi   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(ori) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val | (uint32_t)op_src2->val;
	sprintf(assembly, "ori   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(xori) {

	decode_imm_type(instr);
	reg_w(op_dest->reg) = op_src1->val ^ (uint32_t)op_src2->val;
	sprintf(assembly, "xori   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(beq) {

	decode_imm_type(instr);
	cpu.pc += op_src1->val == op_src2->val ? ((int32_t)(op_src2->imm<<2)) : 0;
	sprintf(assembly, "beq   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(bne) {

	decode_imm_type(instr);
	cpu.pc += op_src1->val != op_src2->val ? ((int32_t)(op_src2->imm<<2)) : 0;
	sprintf(assembly, "bne   %s,   %s,   0x%04x", REG_NAME(op_dest->reg), REG_NAME(op_src1->reg), op_src2->imm);
}

make_helper(bge_or_blt) {

	decode_imm_type(instr);
	if(op_dest->val==0){
	//this is instruction	BLTZ
		if(op_src1->val<0){
			uint32_t target = ((int32_t)((op_src2->imm<<16)>>14));
			cpu.pc += (target+4);
		}
	}else 
	if(op_dest->val==1){
	//this is instruction	BGEZ
		if(op_src1->val>=0){
			uint32_t target = ((int32_t)((op_src2->imm<<16)>>14));
			cpu.pc += (target+4);
		}
	}else 
	if(op_dest->val==17){
	//this is instruction	BGEZAL
		if(op_src1->val>=0){
			uint32_t target = ((int32_t)((op_src2->imm<<16)>>14));
			cpu.pc += (target+4);
		}
		reg_w(31) = cpu.pc+8;
	}else 
	if(op_dest->val==17){
	//this is instruction	BLTZAL
		if(op_src1->val<0){
			uint32_t target = ((int32_t)((op_src2->imm<<16)>>14));
			cpu.pc += (target+4);
		}
		reg_w(31) = cpu.pc+8;
	}
	
}

make_helper(bgtz) {

	decode_imm_type(instr);
	if(op_src1->val>0){
		uint32_t target = ((int32_t)((op_src2->imm<<16)>>14));
		cpu.pc += (target+4);
	}
}

make_helper(blez) {

	decode_imm_type(instr);
	if(op_src1->val<=0){
		uint32_t target = ((int32_t)((op_src2->imm<<16)>>14));
		cpu.pc += (target+4);
	}
}


make_helper(lb) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	int value = (mem_read(addr,1));
	reg_w(op_dest->reg) = (value<<24)>>24;
	sprintf(assembly, "lb   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}

make_helper(lbu) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	uint32_t value = (mem_read(addr,1));
	reg_w(op_dest->reg) = value;
	sprintf(assembly, "lbu   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}

make_helper(lh) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	int value = mem_read(addr,2);
	reg_w(op_dest->reg) = (value<<16)>>16;
	sprintf(assembly, "lh   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}

make_helper(lhu) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	uint32_t value = (mem_read(addr,2));
	reg_w(op_dest->reg) = value;
	sprintf(assembly, "lhu   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}

make_helper(lw) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	uint32_t value = mem_read(addr,4);
	reg_w(op_dest->reg) = value;
	sprintf(assembly, "lw   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}

make_helper(sb) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	uint32_t value = op_dest->val & 0xff;
	mem_write(addr,1,value);
	sprintf(assembly, "sb   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}

make_helper(sh) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	uint32_t value = op_dest->val & 0xffff;
	mem_write(addr,2,value);
	sprintf(assembly, "sh   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}

make_helper(sw) {

	decode_imm_type(instr);
	uint32_t addr = op_src1->val+(((int32_t)op_src2->imm<<16)>>16); 
	uint32_t value = op_dest->val & 0xffffffff;
	mem_write(addr,4,value);
	//printf("write mem: add:%x, value:%x\n",addr,value);
	sprintf(assembly, "sw   %s,  0x%04x(%s)", REG_NAME(op_dest->reg), op_src2->imm,  REG_NAME(op_src1->reg));
}
