/*
Copyright 2021 Elliot-Gaspard COURCHINOUX All rights reserved

---------------The beginning of the  free development license-----------------
This source code is licensed  under the  free development license. An opensource software license. 




See COPYING file to read the free developement license. 

You must agree the license to use this program. 

If you don't agree it, you can't use this software.


---------------The end of free developement license-----------------
*/
struct gaspard_compute_flags {
	
	unsigned char equal;
	unsigned char carry;
	unsigned char overflow;
	unsigned char zero;
	
	
	};
	
struct chiara_system_flag {
	
	unsigned char pagination:1; // bit set if pagination is on 
	unsigned char interrupt:1; // bit set if interrupt is enable 
	
	unsigned char stack:1; // bit on if a pointer stack is here 
	
	unsigned char instruction_pointer:1; // bit on if pointer of instruction is here
	unsigned char exec_mode:1; // bif off = kernel  bit on = restraint mode
	
	
	long long pointer_of_stack;
	
	long long pointer_of_interrupt;
	
	long long pointer_of_pagination_configuration_struct;
	
	};	
struct gaspard_chiara_compute_global {
	
	volatile long long*mem;
	struct gaspard_compute_flags flags;
	struct chiara_system_flag sys_flags;
	 long long GPRS[31];
double FPRS[31];	

int status_stack; // posiiton array in memory for the stack the incrementation 
int elements_in_stack; // the number of elements in stack

unsigned long long position_instack [64] ; // this array says where the GPR/FPR data are in the stack 

unsigned char *instruction_cache; // the pointer that contains instruction from GPR[1] pointer decoded 
int status_cache; // number of bytes in instruction cache
int status_in_cache; // where the parser is in the instruction cache 

unsigned char *jmp_heap;
int status_jmp; // number of bytes in jmp hap
int status_in_jmp; // where the parser is in the instruction cache

unsigned long long adress_jmp;

	};
	struct gaspard_chiara_compute_global global_struct;
static void gaspard_alloc_struct() {
	
	global_struct.mem = malloc(2000000);
	global_struct.elements_in_stack = malloc(4000000); //  4MB of cache
	global_struct.instruction_cache = malloc(4000000); //  
	global_struct.jmp_heap = malloc(4000000); //  jmp heap max 4mb
	}
	#define WRITE 1
#define READ 2
void basic_instruction_parser(unsigned char *,unsigned long );
int chiara_check_autorisation (struct page_table *table,struct page_child *child,char ACTION);
unsigned long get_size_child(int size);
void chiara_build_cache();
void chiara_return_to_instruction_cache();
void chiara_incre_cache();
#define CACHE 1
#define JUMP 2
