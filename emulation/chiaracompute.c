/*
Copyright 2021 Elliot-Gaspard COURCHINOUX All rights reserved

---------------The beginning of the  free development license-----------------
This source code is licensed  under the  free development license. An opensource software license. 




See COPYING file to read the free developement license. 

You must agree the license to use this program. 

If you don't agree it, you can't use this software.


---------------The end of free development license-----------------
*/
 #include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <elf.h>
#include <emul.h>
#include <chiaracompute.h>
#include <limits.h>
#include <tasks.h>
 int bitExtracted2(int number, int k, int p) 
{ 
    return (((1 << k) - 1) & (number >> (p - 1))); 
}
void gaspard_clear_flags( ) {
			global_struct.flags.equal =0;
			global_struct.flags.carry =0;
			global_struct.flags.overflow =0;

}
void gaspard_check_flags_result(long res) {
	if(res<0) {
					global_struct.flags.zero =1;

				}
		if (res & 1<< 31 ) {
			global_struct.flags.carry =1;
			global_struct.flags.overflow =1;

					}
					
					 if ( res < INT_MIN ||  res < INT_MIN) {			
				global_struct.flags.overflow =1;
	}
	
	}
 long long  chiara_get_data(unsigned char reg) {
	
if(reg>=32) {
	
	return global_struct.FPRS[reg];
	
} else {
	
	return	global_struct.GPRS[reg];

	
}		
	
}	
void gaspard_check_flags( long long a, long long b) {
	if(a==b) {
		global_struct.flags.equal =1;
		
		} else {
			
	global_struct.flags.equal =0;

			}
			
	if(a<0) {
					global_struct.flags.zero =1;

				}
	if(b<0) {
					global_struct.flags.zero =1;

				}
				
		if (a & 1<< 63 == b & 1<< 63) {
			global_struct.flags.carry =1;
			global_struct.flags.overflow =1;

					}
					
	 if ( a > LONG_MIN ||  b > LONG_MIN ||  a > LONG_MAX ||  b > LONG_MAX ) {			
				global_struct.flags.overflow =1;
	}
	}
	int gaspard_POP(unsigned char*array,int where) {
printf("pop \n");

unsigned char gpr = array[where+1];


	unsigned long long gpr0_val =  chiara_get_data(0);
		printf("stack adress  %x \n",gpr0_val);

if(0== global_struct.position_instack[gpr]) {
	// interrupt bad instruction/opcode 
	//because the gpr isn't in the stack 
	return where+2;
	}
if(pag_config!=0) {
		unsigned short page = gpr0_val >> 47;
		unsigned char child = gpr0_val >> 51 & 0b1111;
		unsigned long long offset = gpr0_val & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return where+2;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		
			

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return where+2;
}
		
	// controller si l'offset d'incremenrtation en cause pas un manquement dans la page verifie rle soffset 
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return where+2;
}
	if(offset == 0) {
		
 global_struct.mem[ global_struct.position_instack[gpr]] = 0 ;
		
		global_struct.status_stack -= 8;
		 global_struct.elements_in_stack--;
	global_struct.GPRS[0] = page | child << 4 | offset-1 <<44;

 global_struct.position_instack[gpr] = 0;

		
	} else {
		
		
						global_struct.mem[ global_struct.position_instack[gpr]] = 0;
		
		global_struct.status_stack -= 8;
		 global_struct.elements_in_stack--;
	
	
	global_struct.GPRS[0] = page | child << 4 | offset-1 <<44;
	 global_struct.position_instack[gpr] = 0;

	printf("decre gpr because the stack goes up %x \n",global_struct.GPRS[0]);
	
		}
		
		} else {
			
			
global_struct.mem[ global_struct.position_instack[gpr]] = 0;
		
		global_struct.status_stack -= 8;
		 global_struct.elements_in_stack--;
		 global_struct.position_instack[gpr] = 0;
	
			global_struct.GPRS[0] = gpr0_val-1;

			}
	


return where+2;
			
		}

unsigned long get_size_child(int size) {
	switch(size) {
		case 1 : return 512; 
		case 2 : return 1000; 
		case 3 : return 1000000; 
		case 4 : return 20000000; 
		
		
		}
	
	
	}
	
int gaspard_PUSH(unsigned char*array,int where) {
printf("psuh \n");
unsigned char gpr = array[where+1];


	unsigned long long gpr0_val =  chiara_get_data(0);
		printf("stack adress  %x \n",gpr0_val);

	unsigned long long dataempil =  chiara_get_data(gpr);
if(pag_config!=0) {
		unsigned short page = gpr0_val >> 47;
		unsigned char child = gpr0_val >> 51 & 0b1111;
		unsigned long long offset = gpr0_val & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return where+2;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return where+2;
}
		
	// controller si l'offset d'incremenrtation en cause pas un manquement dans la page verifie rle soffset 
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
		
		return where+2; 
		
}
	
	if(offset == 0) {
		
 global_struct.mem[table[page].child[child].adress] =  dataempil;
		
		global_struct.status_stack += 8;
		 global_struct.elements_in_stack++;
	global_struct.GPRS[0] = page | child << 4 | offset+1 <<44;

 global_struct.position_instack[gpr] = table[page].child[child].adress;

		
	} else {
		if(dataempil+offset >= max_offset) {
			// do interrupt because stack error cannot write we don't have much memory to do so 
			return where+2;
			
			}
		
						global_struct.mem[table[page].child[child].adress+offset] = dataempil;
		
		global_struct.status_stack += 8;
		 global_struct.elements_in_stack++;
	
	
	global_struct.GPRS[0] = page | child << 4 | offset+1 <<44;
	 global_struct.position_instack[gpr] = table[page].child[child].adress;

	printf("incre gpr because the stack goes down %x \n",global_struct.GPRS[0]);
	
		}
		
		} else {
			
			
global_struct.mem[gpr0_val] = dataempil;
		
		global_struct.status_stack += 8;
	 global_struct.	elements_in_stack++;
		 global_struct.position_instack[gpr] = gpr0_val;
	
			global_struct.GPRS[0] = gpr0_val+1;

			}
	

return where+2;
			
		}	
		
/***
 * Create the array of jmp instructions for condition
 * */		
void 	chiara_instruction_jump () {
		if(pag_config!=0) {
		unsigned short page = global_struct.adress_jmp >> 47;
		unsigned char child = global_struct.adress_jmp >> 51 & 0b1111;
		unsigned long long offset = global_struct.adress_jmp & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],READ) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return;
}
		
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size

// incre the table ?
// verif child if is not the last child of page node
// verifier is le child présent n'est pas simplement le dernier 

if(pag_config->num_of_page_child>= child ) {
	
global_struct.adress_jmp = page+1 | 0 << 4 | 0 <<44;
// il n'y a plus d'enfants	
} else {
// il reste un enfant
global_struct.adress_jmp = page | child+1 << 4 | 0 <<44;
}
return chiara_instruction_pointer_parser();
// 
}



	if(offset == 0) {
		// les mettre ou dans un cache ? fichier ?
  // instruction sur 8 bits
 	unsigned char *instruction = &global_struct.mem[table[page].child[child].adress];

 	for(int x,z= 0;x<64;x+8,z++) {
			
			global_struct.jmp_heap[global_struct.status_jmp] =instruction[x];
			global_struct.status_jmp++;
			}
 
	global_struct.adress_jmp = page | child << 4 | offset+1 <<44;


		
	} else {
		
		
						 // instruction sur 8 bits

	unsigned char *instruction = &global_struct.mem[table[page].child[child].adress+offset];
	
						for(int x,z= 0;x<64;x+8,z++) {
			
			global_struct.jmp_heap[global_struct.status_jmp] =instruction[x];
			global_struct.status_jmp++;
			}
	global_struct.adress_jmp = page | child << 4 | offset+1 <<44;

		}
		
		} else {
			
			
		 // instruction sur 8 bits
						unsigned char *instruction = &global_struct.mem[global_struct.adress_jmp];
		 
		for(int x,z= 0;x<64;x+8,z++) {
			
			global_struct.jmp_heap[global_struct.status_jmp] =instruction[x];
			global_struct.status_jmp++;
			}
		global_struct.adress_jmp  = global_struct.adress_jmp+1;// incrementation de l'adresse physiquye
		
		
			}
	
	
	
}
		/**
		 * Remplis de 8 octets le cache et incremente comme il se doit GPR1
		 * **/
void chiara_instruction_pointer_parser () {
	unsigned long long gpr1_val =  chiara_get_data(1);
		if(pag_config!=0) {
		unsigned short page = gpr1_val >> 47;
		unsigned char child = gpr1_val >> 51 & 0b1111;
		unsigned long long offset = gpr1_val & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],READ) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return;
}
		
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size

// incre the table ?
// verif child if is not the last child of page node
// verifier is le child présent n'est pas simplement le dernier 

if(pag_config->num_of_page_child>= child ) {
	
global_struct.GPRS[1] = page+1 | 0 << 4 | 0 <<44;
// il n'y a plus d'enfants	
} else {
// il reste un enfant
global_struct.GPRS[1] = page | child+1 << 4 | 0 <<44;
}
return chiara_instruction_pointer_parser();
// 
}



	if(offset == 0) {
		// les mettre ou dans un cache ? fichier ?
  // instruction sur 8 bits
 	unsigned char *instruction = &global_struct.mem[table[page].child[child].adress];

 	for(int x,z= 0;x<64;x+8,z++) {
			
			global_struct.instruction_cache[global_struct.status_cache] =instruction[x];
			global_struct.status_cache++;
			}
 
	global_struct.GPRS[1] = page | child << 4 | offset+1 <<44;


		
	} else {
		
		
						 // instruction sur 8 bits

	unsigned char *instruction = &global_struct.mem[table[page].child[child].adress+offset];
	
						for(int x,z= 0;x<64;x+8,z++) {
			
			global_struct.instruction_cache[global_struct.status_cache] =  instruction[x];
			global_struct.status_cache++;
			}
	global_struct.GPRS[1] = page | child << 4 | offset+1 <<44;

		}
		
		} else {
			
			
		 // instruction sur 8 bits
						unsigned char *instruction = &global_struct.mem[gpr1_val];
		 
		for(int x,z= 0;x<64;x+8,z++) {
			
			global_struct.instruction_cache[global_struct.status_cache] = instruction[x];
			global_struct.status_cache++;
			}
		global_struct.GPRS[1]  = global_struct.GPRS[1]+1;// incrementation de l'adresse physiquye
		
		
			}
	
	
	
}
// create the jump array for branch conditional instruction
void chiara_create_jump_cache(unsigned long long adress){
// create jmp instruction array 	
global_struct.adress_jmp = adress;
		for(int x = 0;x<500000;x++) {
		chiara_instruction_jump();
	}
	
	// executer 
	// vider le cache recommencer etc etc 
	while(500000!=global_struct.status_in_jmp) {
		
			global_struct.status_in_jmp = gaspard_scan_opcode(global_struct.jmp_heap[global_struct.status_in_jmp],global_struct.jmp_heap,global_struct.status_in_jmp );

		// il plantera si par exemple la deuxième partie du disp se situe dans un autre enfant ou page 
		}
	
		global_struct.status_jmp = 0;
		global_struct.status_in_jmp = 0;
		}
	

void chiara_incre_cache(int num) {
	global_struct.status_in_cache+= num;
	
	
	}
// called by ret instruction
 
void chiara_return_to_instruction_cache() {
	global_struct.status_jmp = 0;
		global_struct.status_in_jmp = 0;
		while(500000!=global_struct.status_in_cache) {
		
			global_struct.status_in_cache = gaspard_scan_opcode(global_struct.instruction_cache[global_struct.status_in_cache],global_struct.instruction_cache,global_struct.status_in_cache );

		}
	
	chiara_build_cache();
}
void chiara_build_cache() {
	
	
	if(global_struct.sys_flags.instruction_pointer == 1) {
		for(int x = 0;x<500000;x++) {
		chiara_instruction_pointer_parser();
	}
	
	// executer 
	// vider le cache recommencer etc etc 
	while(500000!=global_struct.status_in_cache) {
		
			global_struct.status_in_cache = gaspard_scan_opcode(global_struct.instruction_cache[global_struct.status_in_cache],global_struct.instruction_cache,global_struct.status_in_cache );

		// il plantera si par exemple la deuxième partie du disp se situe dans un autre enfant ou page 
		}
	
		global_struct.status_cache = 0;
		global_struct.status_in_cache = 0;
		chiara_build_cache();
		}
}	
void basic_instruction_parser(unsigned char *file,unsigned long size) {
int status = 0;

while(status!=size){
	
	//check status of GPR1
	if(global_struct.sys_flags.instruction_pointer == 1) {
		chiara_build_cache();
		} else {
	printf("opcode %d \n",file[status]);
	status = gaspard_scan_opcode(file[status],file,status );
	printf("status %d  \n",status);
}
}
	
	
}
int gaspard_dispIMMmem (unsigned char*array,int where) {
	int globalstatusofarray = 0;
	int status = 0;

	unsigned char opcode = array[where];
globalstatusofarray++;
	

long long datawrite = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
		globalstatusofarray =globalstatusofarray+8;

long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
		globalstatusofarray =globalstatusofarray+8;
chiara_write_mem(adress,datawrite);

		return globalstatusofarray+where;

}
int gaspard_dispGPRmem (unsigned char*array,int where) {
	
		int status = 0;

		int globalstatusofarray = 0;
	unsigned char opcode = array[where];
	printf("instruction opcode %d \n",opcode);
globalstatusofarray++;
	unsigned char info = array[globalstatusofarray+where]; // data to write in the adress in storage on gpr or specified here ?
	printf("46 ? %d \n",info);
	globalstatusofarray++;

				char gpr = info;
				printf("gpr source  %d \n",gpr);

long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
		globalstatusofarray =globalstatusofarray+8;

chiara_write_mem_gpr(adress,gpr);
		return globalstatusofarray+where;
}
int gaspard_emul_dispa(unsigned char*array,int where) {
	int status = 0;

	int globalstatusofarray = 0;
	unsigned char opcode = array[where];
globalstatusofarray++;
	unsigned char info = array[globalstatusofarray+where]; 
	globalstatusofarray++;

				char gpr = info;

long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
		globalstatusofarray =globalstatusofarray+8;

chiara_read_mem_gpr(adress,gpr);
		return globalstatusofarray+where;
			
}

int chiara_check_autorisation (struct page_table *table,struct page_child *child,char ACTION) {
	// check exexec mode 
	if(WRITE== ACTION) {
		if(global_struct.sys_flags.exec_mode == 0) {
		// kernelmode
		
		
			
if(table->mode == 1 || table->mode == 3) {
					// ok the system can access 
					
					
	} else {
	// cannot access
						return 2;
						}
	if(table->disabled == 1) {
		// cannot read or write
		return;
		
		} else if (table->disabled == 2) {
			// cannot write
			return 2;
			
			}
		 else if (table->disabled == 3) {
			// ok, i can write ! !
			
			}
			
			if (table->present == 1) {
				
				// ok I can continue
				} else {
					
					// table disable
					return;
					}
			// now test the child
			
		if(child->disabled_param == table->disabled && child->mode == table->mode) {
			
			// ok same param 
			}	else {
				
				// priority on the param on the child 
				
				
				if(child->mode == 1 || child->mode == 3) {
					// ok the system can access 
					
					
					} else {
						// cannot access
						return 2;
						}
				
				if(child->disabled_param== 3) {
					
					
					} else {
						// cannot write
						return 2;
						}
				}
			
	} else if(global_struct.sys_flags.exec_mode == 1) {
		
			if(table->disabled == 1) {
		// cannot read or write
		return 2;
		
		} else if (table->disabled == 2) {
			// cannot write
			return 2;
			
			}
		 else if (table->disabled == 3) {
			// ok, i can write ! !
			
			}
			
			if (table->present == 1) {
				
				// ok I can continue
				} else {
					
					// table disable
					return 2;
					}
			// now test the child
			
		if(child->disabled_param == table->disabled && child->mode == table->mode) {
			
			// ok same param 
			}	else {
				
				// priority on the param on the child 
				
				
				if(child->mode == 2 || child->mode == 3) {
					// ok the user can access 
					
					
					} else {
						// cannot access
						return 2;
						}
				
				if(child->disabled_param== 3) {
					
					
					} else {
						// cannot write
						return 2;
						}
				}
			
		
		
		}
	
}else {
	
	//


	if(global_struct.sys_flags.exec_mode == 0) {
		// kernelmode
		
		
			
if(table->mode == 1 || table->mode == 3) {
					// ok the system can access 
					
					
	} else {
	// cannot access
						return 2;
						}
	if(table->disabled == 1) {
		// cannot read or write
		return;
		
		} else if (table->disabled == 2) {
			// can read
			
			}
		 else if (table->disabled == 3) {
			//i can't read
						return 2;

			}
			
			if (table->present == 1) {
				
				// ok I can continue
				} else {
					
					// table disable
					return 2;
					}
			// now test the child
			
		if(child->disabled_param == table->disabled && child->mode == table->mode) {
			
			// ok same param 
			}	else {
				
				// priority on the param on the child 
				
				
				if(child->mode == 1 || child->mode == 3) {
					// ok the system can access 
					
					
					} else {
						// cannot access
						return 2;
						}
				
				if(child->disabled_param== 2) {
					
					
					} else {
						// cannot read
						return 2;
						}
				}
			
	} else if(global_struct.sys_flags.exec_mode == 1) {
		
			if(table->disabled == 1) {
		// cannot read or write
		return 2;
		
		} else if (table->disabled == 2) {
			// can read
			
			}
		 else if (table->disabled == 3) {
			// ok, can't read! !
					return 2;
	
			}
			
			if (table->present == 1) {
				
				// ok I can continue
				} else {
					
					// table disable
					return 2;
					}
			// now test the child
			
		if(child->disabled_param == table->disabled && child->mode == table->mode) {
			
			// ok same param 
			}	else {
				
				// priority on the param on the child 
				
				
				if(child->mode == 2 || child->mode == 3) {
					// ok the user can access 
					
					
					} else {
						// cannot access
						return 2;
						}
				
				if(child->disabled_param== 2) {
					
					
					} else {
						// cannot write
						return 2;
						}
				}
			
		
		
		}
	
	
	
}
	
	}

long long chiara_read_mem(long long adress) {
	// return value of a pointer
	
	
	if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],READ) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return;
}
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;
}		
	
	if(offset == 0) {
		
					return global_struct.mem[table[page].child[child].adress];

		
	} else {
		
		
						return global_struct.mem[table[page].child[child].adress+offset]; // *8 because an offset is equal to 64 bits and the adress size is 64 bits in chiara64
	
		}
		
		} else {
			
			
			return global_struct.mem[adress];
			}
	
	
}
void chiara_read_mem_gpr(long long adress,unsigned char  GPs) {
	
	// read memory to gpr/gpr
	
		if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return 0;
}
		
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
	
	if(offset == 0) {
if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = global_struct.mem[table[page].child[child].adress]  ;
	
} else {
 global_struct.GPRS[GPs] = 	global_struct.mem[table[page].child[child].adress]   ;
		

	
}	
		
	} else {
		
		if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = global_struct.mem[table[page].child[child].adress+offset]  ;
	
} else {
 global_struct.GPRS[GPs] = 	global_struct.mem[table[page].child[child].adress+offset]  ;
		

	
}	
		
		
	
	
		}
		
		} else {
	
	if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = global_struct.mem[adress]   ;
	
} else {
	 global_struct.GPRS[GPs] = global_struct.mem[adress] ;
		

	
}	
			
			
			}
	
	
}
void chiara_read_mem_gpr_byte(long long adress,unsigned char  GPs) {
	
	// read memory to gpr/gpr
	
		if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return 0;
}
		
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
		unsigned char ** memory_byte = &global_struct.mem;

	if(offset == 0) {
if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = (*memory_byte[table[page].child[child].adress])  ;
	
} else {
 global_struct.GPRS[GPs] = 	(*memory_byte[table[page].child[child].adress])   ;
		

	
}	
		
	} else {
		
		if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = (*memory_byte[table[page].child[child].adress]+offset) ;
	
} else {
 global_struct.GPRS[GPs] = 	(*memory_byte[table[page].child[child].adress]+offset)   ;
		

	
}	
		
		
	
	
		}
		
		} 
	
	
}
void chiara_read_mem_gpr_short(long long adress,unsigned char  GPs) {
	
	// read memory to gpr/gpr
	
		if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return 0;
}
		
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
		unsigned short ** memory_byte = &global_struct.mem;

	if(offset == 0) {
if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = (*memory_byte[table[page].child[child].adress])  ;
	
} else {
 global_struct.GPRS[GPs] = 	(*memory_byte[table[page].child[child].adress])   ;
		

	
}	
		
	} else {
		
		if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = (*memory_byte[table[page].child[child].adress]+offset) ;
	
} else {
 global_struct.GPRS[GPs] = 	(*memory_byte[table[page].child[child].adress]+offset)   ;
		

	
}	
		
		
	
	
		}
		
		} 
	
	
}
void chiara_read_mem_gpr_long(long long adress,unsigned char  GPs) {
	
	// read memory to gpr/gpr
	
		if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return 0;
}
		
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
		unsigned long ** memory_byte = &global_struct.mem;

	if(offset == 0) {
if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = (*memory_byte[table[page].child[child].adress])  ;
	
} else {
 global_struct.GPRS[GPs] = 	(*memory_byte[table[page].child[child].adress])   ;
		

	
}	
		
	} else {
		
		if(GPs>=32) {
	
	 global_struct.FPRS[GPs] = (*memory_byte[table[page].child[child].adress]+offset) ;
	
} else {
 global_struct.GPRS[GPs] = 	(*memory_byte[table[page].child[child].adress]+offset)   ;
		

	
}	
		
		
	
	
		}
		
		} 
	
	
}

void chiara_write_mem_byte(long long adress,char data) {
	
		if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return;
}

	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
	unsigned char ** memory_byte = &global_struct.mem;
	if(offset == 0) {
		(*memory_byte[table[page].child[child].adress]) = data;

		
	} else {
		
(*memory_byte[table[page].child[child].adress+offset]) = (unsigned char) data;		
	
		}
		
		} 
	
	
	}
void chiara_write_mem_short(long long adress,short data) {
	
		if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return;
}

	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
	unsigned short ** memory_byte = &global_struct.mem;
	if(offset == 0) {
		(*memory_byte[table[page].child[child].adress]) = data;

		
	} else {
		
(*memory_byte[table[page].child[child].adress+offset]) =data;		
	
		}
		
		} 
	
	
	}
void chiara_write_mem_long(long long adress,long data) {
	
		if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return;
}

	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
	unsigned long ** memory_byte = &global_struct.mem;
	if(offset == 0) {
		(*memory_byte[table[page].child[child].adress]) = data;

		
	} else {
		
(*memory_byte[table[page].child[child].adress+offset]) =data;		
	
		}
		
		} 
	
	
	}
void chiara_write_mem(long long adress,long long data) {
	// ecrire valeur immediate 64 dans pointeur
	if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return;
}

	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}		
	
	if(offset == 0) {
		global_struct.mem[table[page].child[child].adress] = data;

		
	} else {
		
global_struct.mem[table[page].child[child].adress+offset] =data;		
	
		}
		
		} else {
			
			global_struct.mem[adress] = data;
			
			}
	
}
void chiara_write_mem_gpr(long long adress,unsigned char  GPs) {
	// write gor data to pointer
	
	if(pag_config!=0) {
		unsigned short page = adress >> 47;
		unsigned char child = adress >> 51 & 0b1111;
		unsigned long long offset = adress & 0b11111111111111111111111111111111111111111111;
		
		if(child>pag_config->num_of_page_child|| page>pag_config->page_table_num) {
			// because the pagination table or the child doesn't exist do interrupt 
			return 0;
			
			}
	struct page_table *table = (struct page_table *)  &global_struct.mem[pag_config->adress_of_page] ; 
		

if(chiara_check_autorisation(&table[page],&table[page].child[child],WRITE) == 2) {
	// do interrupt 2 because I can't access in write or read or I don't have the autorization
	return 0;
}
		
	unsigned long  size_of_child = get_size_child(pag_config->child_size);
	
	unsigned long max_offset =  size_of_child /8;
	
	if(table[page].child[child].adress+offset > table[page].child[child].adress+max_offset) {
				// offset is too large for the child size
return 0;

}	
	if(offset == 0) {
if(GPs>=32) {
	
	global_struct.mem[table[page].child[child].adress] = global_struct.FPRS[GPs] ;
	
} else {
	global_struct.mem[table[page].child[child].adress] = global_struct.GPRS[GPs] ;
		

	
}	
		
	} else {
		
		if(GPs>=32) {
	
	global_struct.mem[table[page].child[child].adress+offset] = global_struct.FPRS[GPs] ;
	
} else {
	global_struct.mem[table[page].child[child].adress+offset] = global_struct.GPRS[GPs] ;
		

	
}	
		
		
	
	
		}
		
		} else {
	
	if(GPs>=32) {
	
	global_struct.mem[adress] = global_struct.FPRS[GPs] ;
	
} else {
	global_struct.mem[adress] = global_struct.GPRS[GPs] ;
		

	
}	
			
			
			}
	
	
}
void chiara_write_data_immediate(unsigned char reg,unsigned long long 	data) {
	


	
if(reg>=32) {
	
	global_struct.FPRS[reg] = data;
	
} else {
	
		global_struct.GPRS[reg] = data;

	
}	
	
}
void chiara_write_data(unsigned char reg,unsigned char	reg2) {
	
if(reg>=32) {
	
	global_struct.FPRS[reg] = global_struct.FPRS[reg2];
	
} else {
	
		global_struct.GPRS[reg] = global_struct.GPRS[reg2];

	
}	
	
	
}


int gaspard_emul_disp (unsigned char*array,int where) {
	
	int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
			unsigned char destination = array[where+status];
			status++;




	if(destination!=66) {
		if(destination == 0) {
			global_struct.sys_flags.pointer_of_stack = 1;

			} else if(destination == 1) {
				
			global_struct.sys_flags.instruction_pointer = 1;
				

				}
		 long long  data = chiara_get_data(destination);
 chiara_write_data(source,data);

 	return where+status; 

		} else {
			
			 long long datawriteinsource =  array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
						status = status +8;


chiara_write_data_immediate(source,datawriteinsource);
	return where+status; 

			}
	
	}	
void gaspard_emul_xor_gpr (unsigned char source, unsigned char destination) {
	gaspard_clear_flags();
	 long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	 
	gaspard_check_flags(sourcedata,destinationdata);
	long long data  = sourcedata^destinationdata;
	gaspard_check_flags_result(data);
	
	
	 chiara_write_data(destination,data);

	}
void gaspard_emul_or_gpr (unsigned char source, unsigned char destination) {
	gaspard_clear_flags();
 long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);

	long long data = sourcedata|destinationdata;

		gaspard_check_flags_result(data);
 chiara_write_data(destination,data);
	}
void gaspard_emul_and_gpr (unsigned char source, unsigned char destination) {
		gaspard_clear_flags();

	long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);

		long long data = sourcedata&destinationdata;
		 chiara_write_data(destination,data);

	}
void gaspard_emul_not_gpr (unsigned char destination) {
			gaspard_clear_flags();
long long  sourcedata = chiara_get_data(destination);
		gaspard_check_flags(sourcedata,0);

	~sourcedata;
		gaspard_check_flags_result(sourcedata);
		 chiara_write_data(destination,sourcedata);

	}
void gaspard_emul_multiply_gpr (unsigned char source, unsigned char destination) {
			gaspard_clear_flags();

	long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);

	long long data  = sourcedata*destinationdata;
		gaspard_check_flags_result(data);
		 chiara_write_data(destination,data);

	}
void gaspard_emul_add_gpr (unsigned char source, unsigned char destination) {
			gaspard_clear_flags();

long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);
	
	long long data  = sourcedata+destinationdata;
		gaspard_check_flags_result(data);
		 chiara_write_data(destination,data);

	}
void gaspard_emul_div_gpr (unsigned char source, unsigned char destination) {
			gaspard_clear_flags();

long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);
	
	long long data  = (unsigned long long) sourcedata/(unsigned long long) destinationdata;
		gaspard_check_flags_result(data);
		 chiara_write_data(destination,data);

	}
void gaspard_emul_div_signed_gpr (unsigned char source, unsigned char destination) {
			gaspard_clear_flags();

long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);
	
	long long data  = (signed long long) sourcedata/(signed long long) destinationdata;
		gaspard_check_flags_result(data);
		 chiara_write_data(destination,data);

	}
void gaspard_emul_susbtract_gpr (unsigned char source, unsigned char destination) {
			gaspard_clear_flags();

	long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);
	
	long long data = sourcedata-destinationdata;
		gaspard_check_flags_result(data);
		 chiara_write_data(destination,data);

	}
void gaspard_emul_shl_gpr (unsigned char source, unsigned char destination) {
			gaspard_clear_flags();
long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);
	
	
	long long data = destinationdata<<sourcedata;

		gaspard_check_flags_result(data);
		 chiara_write_data(destination,data);

	}
void gaspard_emul_shr_gpr (unsigned char source, unsigned char destination) {
			gaspard_clear_flags();
long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);
	
	
	long long data = destinationdata>>sourcedata;

		gaspard_check_flags_result(data);
		 chiara_write_data(destination,data);

	}
void gaspard_emul_increment_gpr (unsigned char destination) {
				gaspard_clear_flags();
long long  sourcedata = chiara_get_data(destination);

			gaspard_check_flags(sourcedata,0);

	sourcedata++;
		gaspard_check_flags_result(sourcedata);
		 chiara_write_data(destination,sourcedata);

	}
void gaspard_emul_decrement_gpr (unsigned char destination) {
				gaspard_clear_flags();
long long  sourcedata = chiara_get_data(destination);

			gaspard_check_flags(sourcedata,0);

	sourcedata--;
		gaspard_check_flags_result(sourcedata);
		 chiara_write_data(destination,sourcedata);
	}
void gaspard_emul_compare(unsigned char source, unsigned char destination) {
	
		gaspard_clear_flags();
long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
			gaspard_check_flags(sourcedata,destinationdata);
	}
  
  void gaspard_emul_modul_gpr (unsigned char source, unsigned char destination) {
				gaspard_clear_flags();

long long  sourcedata = chiara_get_data(source);
	 long long  destinationdata = chiara_get_data(destination);
	gaspard_check_flags(sourcedata,destinationdata);

	long long data = sourcedata%destinationdata;
		gaspard_check_flags_result(data);
		
		 chiara_write_data(destination,data);


	}
