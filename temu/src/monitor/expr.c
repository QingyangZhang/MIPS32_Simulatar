#include "temu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	NOTYPE = 256, EQ, NEQ, DEC_NUM, HEX_NUM, NEG, DEREF, REG

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	/*My code here*/
	{"0x[A-Fa-f0-9]+",HEX_NUM},
	{"([1-9][0-9]{1,31})|[0-9]",DEC_NUM},
	{"-",'-'},				// Decrease
	{"\\*", '*'},				// Multiply
	{"/", '/'},				// Divide
	{"\\(",'('},				// Left parenthesis
	{"\\)",')'},				// Right parenthesis
	{"\\$[a-z0-9]+",REG},			// Reg
	/*My code end*/	

	{" +",	NOTYPE},			// spaces
	{"\\+", '+'},				// plus
	{"==", EQ},				// equal
	{"!=", NEQ}				// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					/*My code begin*/
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case EQ:
					case NEQ:
					case HEX_NUM:
					case DEC_NUM:
					case REG:
					{
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token++].str[substr_len] = '\0';
						break;
					}
					case NOTYPE:
						break;
					/*My code end*/
					default: panic("please implement me");
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("No match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

int check_parentheses(Token *p,Token *q){
	int count = 0;
	int sign = 0;
	for (Token *iter_p=p;iter_p<=q;iter_p++) {
		if (iter_p->type=='(')
			count++;
		else if (iter_p->type==')')
			count--;
		if (count<0) {
		sign=1;
		break;
		}
	}
	if(count!=0||sign!=0){
		return 2;
	}
	if (sign==0 && count==0 && (p->type=='(' && q->type==')'))
		return 0;
	return 1;

}

Token* dominant_op(Token *p,Token *q){
	int is_inP=0;
    	int sign=0;	//record the priority
	Token *dominant_op=NULL;
	//Noted that the associative law of unary operators is different with binary operator
	//Here, the priorities is setted according to C.
	//Visit https://baike.baidu.com/item/%E8%BF%90%E7%AE%97%E7%AC%A6%E4%BC%98%E5%85%88%E7%BA%A7/4752611?fr=aladdin#3 for more details.
	for (Token *iter_p=p;iter_p<=q;iter_p++) {
		if (iter_p->type=='(') {
			is_inP++;
			continue;
		}
		if (iter_p->type==')') {
			is_inP--;
			continue;
		}
		if (is_inP!=0) continue;
		
		if (iter_p->type!='+' && iter_p->type!='-' 
		   && iter_p->type!='*' && iter_p->type!='/'
		   && iter_p->type!=NEG && iter_p->type!=DEREF
		   && iter_p->type!=EQ && iter_p->type!=NEQ
			)
			continue;
		if (sign<=7 && (iter_p->type == EQ || iter_p->type == NEQ)){
			dominant_op=iter_p;
			sign=7;
		}
        	else if (sign<=4 && (iter_p->type =='+' || iter_p->type =='-')){
			dominant_op=iter_p;
			sign=4;
		}
		else if (sign<=3 && (iter_p->type =='*' || iter_p->type =='/')){
			dominant_op=iter_p;
			sign=3;
		}
		else if(sign<2 && (iter_p->type==NEG||iter_p->type==DEREF)){
			//unary operator caculate from the right side
			//thus the dominant operator is on the left side
			dominant_op=iter_p;
			sign=2;
		}
	}
	return dominant_op;
}

int eval(Token*p,Token*q,bool*success){
	if(p>q){
		printf("Bad expression\n");
		*success=false;
		return 0;
	}
	else if(p==q){
		switch(p->type){
		case DEC_NUM:{
			if(p->str[0]>'9'||p->str[0]<'0'){
				*success=false;
				printf("Bad expression\n");		
			}
			return (int)atoi(p->str);
			}
		case HEX_NUM:{
			int num=0;
			if(!sscanf(p->str, "%x", &num)){
				printf("Valid HEX Number\n");
				*success=false;
			}
			printf("try convert hex\n");
			return num;
			}
		case REG:{
			char*reg=p->str;
			for(int i=0;i<32;i++){
				if(!strcmp(reg,regfile[i])){
				return cpu.gpr[i]._32;
				}
			}
			if(!strcmp(reg,"$pc"))return cpu.pc;
			else if(!strcmp(reg,"$hi"))return cpu.hi;
			else if(!strcmp(reg,"$lo"))return cpu.lo;
			else{
				printf("No such REG\n");
				*success=false;
				return 0;
				}					
			}
		}
	}
	else if(check_parentheses(p,q)==0){
		return eval(p+1,q-1,success);
	}
	else if(check_parentheses(p,q)==2){
		*success=false;
		printf("Error,token didn't match.\n");
		return 0;	
	}
	else{
		int val1=1,val2;
		Token *op = dominant_op(p, q);
		Assert(op,"exp analysis failed\n");
		//unary operator
		val2 = eval(op + 1, q,success);
		switch(op->type){
			case NEG: return -val2;
			case DEREF: return mem_read(val2,4);
	
		}
		val1 = eval(p, op - 1,success);
		switch (op->type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case  EQ: return val1 == val2;
			case NEQ: return val1 != val2;  
    	}
	}
	return 0;
}

uint32_t expr(char *e, bool *success) {
	*success=true;
	if(!make_token(e)) {
		*success = false;
		printf("Fail to make token.\n");
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	/*My code begin*/
	for(int i=0;i<nr_token;i++){
		if(tokens[i].type=='-'&&(i==0||tokens[i-1].type=='(')){
			tokens[i].type = NEG;
		}
		if(tokens[i].type=='*'&&(i==0||(tokens[i-1].type!=')'&&tokens[i-1].type!=DEC_NUM&&tokens[i-1].type!=HEX_NUM&&tokens[i-1].type!=REG))){
			tokens[i].type = DEREF;
		}
	}
	return eval(&tokens[0],&tokens[nr_token-1],success);
	/*My code end*/
	panic("please implement me");
	return 0;
}

