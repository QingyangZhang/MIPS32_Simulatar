#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

WP* gethead(){
	return head;
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

void free_wp(WP*wp){
	//clear exp comments
	wp->exp[0]='\0';
	//inset into free list
	wp->next=free_;
	free_=wp;
}
/*My code end*/

