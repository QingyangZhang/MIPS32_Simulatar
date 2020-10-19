#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

//interface of wp list
WP* get_head(){
	return head;
}


void display_wp(){
	int count = 0;
	for(WP*itor=head;itor!=NULL;itor=itor->next){
		printf("NO.%d watchpoint, Exp: %s, Value: %d\n",itor->NO, itor->exp, itor->last_value);
	}
	printf("%d watchpoints working now\n",count);
}

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
/*My code begin*/
WP* new_wp(){
	if(free_){
		//release
		WP* new_wp=free_;
		free_=free_->next;
		//insert
		new_wp->next=head;
		head=new_wp;
		return new_wp;
	}
	printf("No available free watchpoint\n");
	return NULL;
	
}

int free_wp(WP*wp){
	if(wp!=head){
		//find the node before
		WP*itor=head;
		for(;itor->next!=wp;itor=itor->next){	
		}
		if(itor==NULL){
			printf("Fail to free: cannot find watchpoint.\n");
			return -1;
		}
		//delete wp from the list
		itor->next=wp->next;
	}else{
		head=wp->next;	
	}
	//clear exp comments
	wp->exp[0]='\0';
	//inset into free list
	wp->next=free_;
	free_=wp;
	return 0;
}
/*My code end*/

