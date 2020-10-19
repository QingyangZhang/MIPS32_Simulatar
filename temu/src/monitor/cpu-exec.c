#include "monitor.h"
#include "helper.h"
/*My code begin*/
#include "watchpoint.h"
#include <stdlib.h>
/*My code end*/
/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
/*My code begin*/
WP*get_head();
uint32_t expr(char *e, bool *success);
/*My code end*/
#define MAX_INSTR_TO_PRINT 10

int temu_state = STOP;

void exec(uint32_t);

char assembly[80];
char asm_buf[128];

void print_bin_instr(uint32_t pc) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", pc);
	for(i = 3; i >= 0; i --) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(pc + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 8, "");
}

/* Simulate how the MiniMIPS32 CPU works. */
void cpu_exec(volatile uint32_t n) {
	if(temu_state == END) {
		printf("Program execution has ended. To restart the program, exit TEMU and run again.\n");
		return;
	}
	temu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	for(; n > 0; n --) {
#ifdef DEBUG
		uint32_t pc_temp = cpu.pc;
		if((n & 0xffff) == 0) {
			
			fputc('.', stderr);
		}
#endif

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		
		exec(cpu.pc);
		cpu.pc += 4;

#ifdef DEBUG
		print_bin_instr(pc_temp);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		}
#endif

		/* TODO: check watchpoints here. */
		/*My code begin*/
		bool*success=(bool*)malloc(sizeof(bool));
		WP*itor=get_head();
		int val=0;
		while(itor!=NULL){
			val=expr(itor->exp,success);
			if(val!=itor->last_value){
			printf("Watchpoint triggered: TEMU stop because the exp %s changed from %x to %x\n",itor->exp,itor->last_value,val);
			itor->last_value=val;
			temu_state = STOP;
			}
			itor=itor->next;
		}
		/*My code end*/
		if(temu_state != RUNNING) { return; }
	}

	if(temu_state == RUNNING) { temu_state = STOP; }
}
