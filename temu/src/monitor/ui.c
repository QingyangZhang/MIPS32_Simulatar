#include "monitor.h"
#include "temu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <watchpoint.h>

void cpu_exec(uint32_t);

void display_reg();

uint32_t mem_read(uint32_t addr, size_t len);

/*My code begin*/
uint32_t expr(char*e,bool*success);
WP* new_wp();
int free_wp(WP*wp);
WP* get_head();
void display_wp();
/*My code end*/

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(temu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}
/*My code begin*/
static int cmd_si(char *args) {
	if(args){
		int steps=atoi(args);
		if(steps>0){
			printf("%s steps.\n",args);
			cpu_exec(steps);
		}else{
			printf("Error, invaild args.\n");
		}
	}else{
		printf("Single step.\n");
		cpu_exec(1);		
	}
	return 0;
}

static int cmd_info_r(){
	printf("Show the statuses of regs\n");	
	display_reg();
	return 0;
}

static int cmd_info_w(){
	printf("Show the statuses of watchpoints\n");	
	display_wp();
	return 0;
}

static int cmd_info(char *args){
	if(!args){
		printf("Error, need args.\n");
		return 0;
	}
	if(strcmp(args, "r") == 0){
		cmd_info_r();
		return 0;
	}else if(strcmp(args, "w") == 0){
		cmd_info_w();
		return 0;
	}else{
		printf("Error, invaild args.\n");
		return 0;
	}
	printf("Error, unknow reason.\n");
	return 0;
}

static int cmd_x(char *args){
	if(!args){
		printf("Error, invaild args.\n"); 
		return 0;
	}
	char *N = strtok(args, " ");
	char *EXPR = N + strlen(N) + 1;
	if(N&&EXPR){	
		printf("x N = %s, EXPR = %s\n",N,EXPR);
		uint32_t data = 0;
    		uint32_t addr = 0;
		int ndwords = atoi(N);
		if(ndwords<1){
			printf("Error, N must be positive.\n");
			return 0;
		}         
    		if(sscanf(EXPR, "%x", &addr)<1){
			printf("Error, x must be valid hex number.\n");
			return 0;
		}
		printf("Read %d double words from 0x%08x.\n",ndwords,addr);		
		for(int i=0;i<ndwords;i++){
			data=mem_read(addr,4);
			addr+=4;
			printf("0x%08x\n",data);
		}    
		return 0;
	}else{
		printf("Error, invaild args.\n");
		return 0;
	}
}

static int cmd_p(char *args){
	if(!args){
		printf("Error, invaild args.\n"); 
		return 0;
	}
	bool*success=(bool*)malloc(sizeof(bool));
	*success=true;
	uint32_t value=expr(args,success);
	if(!(*success)){
		printf("Fail to caculate.\n");
	}else{
		printf("the value of exp is 0x%x\n",value);
		printf("the value of exp is %d\n",value);
	}
	return 0;
}

static int cmd_w(char *args){
	if(!args){
		printf("Error, invaild args.\n"); 
		return 0;
	}
	//acquire new wp
	WP*wp=new_wp();
	//set exp
	strcpy(wp->exp,args);
	bool*success=(bool*)malloc(sizeof(bool));
	*success=true;
	wp->last_value=expr(wp->exp,success); 
	printf("Watchpoint: New watchpoint assigned with init value: %x\n",wp->last_value); 
	return 0;
}

static int cmd_d(char *args){
	if(!args){
		printf("Error, invaild args.\n"); 
		return 0;
	}
	int no=0;
	if(sscanf(args, "%d", &no)<1){
		printf("Error, args must be valid number.\n");
		return 0;
	}
	//Alloc new wp
	WP*itor = get_head();
	for(;itor!=NULL;itor=itor->next){
		if(itor->NO==no)break;
	}
	if(itor==NULL){
		printf("Error: Cannot find watchpoint.\n");
		return 0;
	}
	printf("find watchpoint.\n");
	//free wp
	if(free_wp(itor)){
		printf("Error: Fail to free watchpoint.\n");
		return 0;
	};
	printf("Watchpoint: delete watchpoint NO.%d\n",no); 
	return 0;
}
/*My code end*/
static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	/* TODO: Add more commands */
	/*My code begin*/
	{ "si", "si command", cmd_si },
	{ "info", "info command", cmd_info },
	{ "x", "x command", cmd_x },
	{ "p", "p command", cmd_p },
	{ "w", "w command", cmd_w },
	{ "d", "d command", cmd_d },
	/*My code end*/
		
	{ "q", "Exit TEMU", cmd_q }

	

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd);printf("Unknown command '%s'\n", cmd);}
	}
}
