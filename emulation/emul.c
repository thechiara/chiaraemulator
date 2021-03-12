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

int gaspard_func(unsigned char*array,int where) {
	printf("func gaspard \n");
	int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned short subcommand = array[where+status] | array[where+status+1] << 8;
	 status = status+2;
	 char *functioninstruction = malloc(sizeof(char));
	 int functioninstructionwhere = 0;
	while(1) {
		if(array[status+where] == 17) {
			status++;
		functioninstruction[functioninstructionwhere] = 110;
	functioninstructionwhere++;		

			break;
			}
			functioninstruction[functioninstructionwhere] = array[status+where];
			// il y a un rpobeleme
	status++;		
	functioninstructionwhere++;		
			
		}
//~ printf("where+Status %d \n",where+status);
//~ gaspard_save_function_stack(functioninstruction,subcommand);
	return where+status;
	}

void chiara_int_pag() {
	
	chiara64_interrupt  = (struct interrupt *) &global_struct.mem[global_struct.sys_flags.pointer_of_interrupt];
	
for(int x = 0;x<3;x++) {
	
	if(chiara64_interrupt[x].active == 1) {
		printf("The essential  interrupt num %d  is active ! \n ",x);
		
		} else {
	printf("The essential  interrupt num %d  is not active ! \n ",x);

			
			}
	
	}	
}


void chiara_call_interrupt_handler(int num) {
	
struct interrupt *int_c =  &chiara64_interrupt[num];


if(int_c->active == 1) {
	chiara_create_jump_cache(int_c->adress);

	
} else {
	
printf("interrupt %d is not active %d %s",num,__LINE__,__FILE__);	
	
}
	
	
}

void chiara_resolve_pag() {
	
	// read the parameter pagination table 
	
	pag_config = (struct pagination_general_configuration *) &global_struct.mem[global_struct.sys_flags.pointer_of_pagination_configuration_struct];
	
	printf("hardcoded in mem val for pag config %x \n",global_struct.mem[global_struct.sys_flags.pointer_of_pagination_configuration_struct]);
	
printf("pag_config adress %x \n",pag_config);
printf("the number of page child a page table have : %d \n",pag_config->num_of_page_child);


printf("the actual number of page table %d \n",pag_config->page_table_num);
switch(pag_config->child_size) {
	
	case 1 : printf("size of child  : 512 bytes \n"); break;
	case 2 : printf("size of child : 1 kilobytes \n"); break;
	case 3 : printf("size of child : 1 megabytes \n"); break;
	case 4 : printf("size of child : 20 megabytes \n"); break;
	
	default : printf("fatal error I can't undersand the size of child pagination will be disabled \n"); global_struct.sys_flags.pagination = 0;  global_struct.sys_flags.pointer_of_pagination_configuration_struct = 0; return;
	}

	printf("pagination is now active ! \n");


}

int gaspard_prcfg (unsigned char*array,int where) {
	int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char subcommand = array[where+status];
	status++;
	
	switch (subcommand) {
		case 1: {
			printf("INT CONFIG \N");
			long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
		status =status+8;
		if(global_struct.sys_flags.exec_mode == 0) {
		if(adress = 0) {
			
			
			global_struct.sys_flags.interrupt = 0; // disable 
			global_struct.sys_flags.pointer_of_interrupt = 0; // disable 
			
} else {
	
		global_struct.sys_flags.interrupt = 1; // enable 
			global_struct.sys_flags.pointer_of_interrupt = adress; // enable 
		chiara_int_pag();

}
}
			//
			break;
			
			}
		case 2: {
			printf("pagination\n");
			unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
		status =status+8;
				if(global_struct.sys_flags.exec_mode == 0) {

				if(adress = 0) {
			
			
			global_struct.sys_flags.pagination = 0; // disable 
			global_struct.sys_flags.pointer_of_pagination_configuration_struct = 0; // disable 
			
} else {
	printf("value of adress %x \n",adress);
		global_struct.sys_flags.pagination = 1; // enable 
			global_struct.sys_flags.pointer_of_pagination_configuration_struct = adress; // enable 
		chiara_resolve_pag();

}
		}
		
		
			break;
			}
		case 3: {
			printf("exec mode \N");
		
		
		unsigned char mode = array[where+status] ;
		status++;
		
				if(global_struct.sys_flags.exec_mode == 0) {

		if(mode==2) {
			
			global_struct.sys_flags.exec_mode |= 1;
			// restraint mode activated 
			}
		}
			break;
			
			} 
			
			
			default : printf("bad opcode malformed prcfg \n"); break;
			
	
		
		
		
		}
	
	return status+where;
}


	int gaspard_dispa(unsigned char*array,int where) {
printf("dispa \n");
return gaspard_emul_dispa(array,where);
			
		}
	int gaspard_ret(unsigned char*array,int where) {
printf("ret \n");
chiara_return_to_instruction_cache();
return where+1;
			
		}

int gaspard_disp(unsigned char*array,int where) {
	printf("disp \n");
	return gaspard_emul_disp(array,where);
	
}
int gaspard_calle(unsigned char*array,int where) {
	printf("CALLE \n");
	int status = 0;
	unsigned char opcode = array[where+status];
	status++;
	unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;
	 
	 // execute code ! 
	if(global_struct.flags.equal == 1) {
			 chiara_incre_cache(1+8); // 9 bytes pass opcode and adress
chiara_create_jump_cache(adress);
		// if the previous instruction is equal jump to source
		printf("equal \n");
	//	gaspard_call_to(subcommand);
		} else {
					return where+status; 

			// do nothing 
			}

}
int gaspard_callz(unsigned char*array,int where) {
	printf("CALLz \n");
	int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;
	if(global_struct.flags.zero == 1) {
		// if the previous instruction is equal jump to source
		 chiara_incre_cache(1+8); // 9 bytes pass opcode and adress
chiara_create_jump_cache(adress);
		printf("zero \n");
		//~ gaspard_call_to(subcommand);
		} else {
			
			// do nothing 
			}
		return where+status; 

}
int gaspard_callrz(unsigned char*array,int where) {
	printf("CALLRz \n");
	int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;
	if(global_struct.flags.zero == 1) {
		 chiara_incre_cache(1+8); // 9 bytes pass opcode and adress
chiara_create_jump_cache(adress);
		printf("zero \n");
		//~ gaspard_call_to(subcommand);
		} else {
			
			// do nothing 
			}
		return where+status; 

}
int gaspard_callc(unsigned char*array,int where) {
	printf("CALLC \n");
	int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;
	if(global_struct.flags.carry == 1) {
		// if the previous instruction is equal jump to source
		 chiara_incre_cache(1+8); // 9 bytes pass opcode and adress
chiara_create_jump_cache(adress);
		printf("zero \n");
		//~ gaspard_call_to(subcommand);
		} else {
			
			// do nothing 
			}
		return where+status; 

}
int gaspard_callo(unsigned char*array,int where) {
	printf("CALLO \n");
	int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;
	if(global_struct.flags.overflow == 1) {
		// if the previous instruction is equal jump to source
		 chiara_incre_cache(1+8); // 9 bytes pass opcode and adress
chiara_create_jump_cache(adress);
		//~ gaspard_call_to(subcommand);
		} else {
			
			// do nothing 
			}
		return where+status; 

}
int gaspard_callro(unsigned char*array,int where) {
	printf("CALLRO \n");
	int status = 0;
	unsigned char opcode = array[where+status];
	status++;
	unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;
	if(global_struct.flags.overflow == 1) {
 chiara_incre_cache(1+8); // 9 bytes pass opcode and adress
chiara_create_jump_cache(adress);
		} else {
			
			// do nothing 
			}
		return where+status; 

}
int gaspard_callrc(unsigned char*array,int where) {
	printf("CALLRC \n");
	int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;
	if(global_struct.flags.carry == 1) {
		// if the previous instruction is equal jump to source
	 chiara_incre_cache(1+8); // 9 bytes pass opcode and adress
chiara_create_jump_cache(adress);
		//~ gaspard_call_to(subcommand);
		} else {
			
			// do nothing 
			}
		return where+status; 

}
int gaspard_add(unsigned char*array,int where) {
	printf("add \n");
	int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
	// code here
	
	gaspard_emul_add_gpr(source,destination);
	return where+status; 

}
int gaspard_ADDC(unsigned char*array,int where) {
	printf("addc \n");
	int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
	// code here
	
	gaspard_emul_add_gpr(source,destination);
	return where+status; 

}
int gaspard_inc(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		
		gaspard_emul_increment_gpr(source);

	return where+status; 
	
}
int gaspard_sub(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
		gaspard_emul_susbtract_gpr(source,destination);

	return where+status; 
}
int gaspard_SUBC(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
		gaspard_emul_susbtract_gpr(source,destination);

	return where+status; 
}
int gaspard_modul(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
		gaspard_emul_modul_gpr(source,destination);

	return where+status; 
}
int gaspard_div(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
		gaspard_emul_div_gpr(source,destination);

	return where+status; 
}
int gaspard_divs(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
		gaspard_emul_div_signed_gpr(source,destination);

	return where+status; 
}
int gaspard_mult(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
		gaspard_emul_multiply_gpr(source,destination);
	return where+status; 
}
int gaspard_xor(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
			gaspard_emul_xor_gpr(source,destination);

	return where+status; 
}
int gaspard_and(unsigned char*array,int where) {
	printf("AND \n");
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
				gaspard_emul_and_gpr(source,destination);

	return where+status; 
}
int gaspard_or(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
					gaspard_emul_or_gpr(source,destination);

	return where+status; 
}
int gaspard_not(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;

						gaspard_emul_not_gpr(source);

	return where+status; 
}
int gaspard_shl(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
	gaspard_emul_shl_gpr(source,destination);
		printf("shift left \n");

	return where+status; 
}
int gaspard_shr(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
		gaspard_emul_shr_gpr(source,destination);

		printf("shift right \n");

	return where+status; 
}
int gaspard_cmp(unsigned char*array,int where) {
		int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
	gaspard_emul_compare(source,destination);
	return where+status; 
	printf("shift right \n");
}
int gaspard_config(unsigned char*array,int where) {
	printf("Config  \n");
			int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		unsigned char destination = array[where+status];
	status++;
	return where+status; 
}
int gaspard_decrement(unsigned char*array,int where) {
	printf("gaspard_decrement \n");
			int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;

			gaspard_emul_decrement_gpr(source);

	return where+status; 
}
int gaspard_syscall(unsigned char*array,int where) {
	printf("SYSCALL \n");
			int status = 0;
	unsigned char opcode = array[where];
	status++;
	unsigned char source = array[where+status];
	status++;
		 chiara_incre_cache(2); 

chiara_call_interrupt_handler(source);
			// call handler

// return or not return ?
}

int gaspard_dispab_write(unsigned char*array,int where) {
	printf("DISPAB WRITE \n");
int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;

unsigned char byte_data  =  array[where+status];
chiara_write_mem_byte(adress,byte_data);
status++;
return where+status;
}

int gaspard_dispas_write(unsigned char*array,int where) {
	printf("DISPAS WRITE \n");
int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;

unsigned short short_data  =  array[where+status] | array[where+status+1] << 8;
status = status+1;
chiara_write_mem_short(adress,short_data);
return where+status;
}

int gaspard_dispal_write(unsigned char*array,int where) {
	printf("DISPAL WRITE \n");
int status = 0;
	unsigned char opcode = array[where+status];
	status++;
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;

unsigned long long_data  =  array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24;
status = status+4;
chiara_write_mem_long(adress,long_data);
return where+status;
}

int gaspard_dispab_read(unsigned char*array,int where) {
	printf("DISPAB READ \n");
int status = 0;
	unsigned char opcode = array[where+status];
	status++;
	
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;

unsigned char GPR_FPR = array[where+status];
	status++;
	chiara_read_mem_gpr_byte(adress,GPR_FPR);
return where+status;

}

int gaspard_dispas_read(unsigned char*array,int where) {
	printf("DISPAS READ \n");
int status = 0;
	unsigned char opcode = array[where+status];
	status++;
	
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;

unsigned char GPR_FPR = array[where+status];
	status++;
		chiara_read_mem_gpr_short(adress,GPR_FPR);

return where+status;

}
int gaspard_dispal_read(unsigned char*array,int where) {
	printf("DISPAL READ \n");
int status = 0;
	unsigned char opcode = array[where+status];
	status++;
	
		unsigned long long adress = array[where+status] | array[where+status+1] << 8  |array[where+status+2] << 16 |   array[where+status+3] << 24 | array[where+status+4] << 32 | array[where+status+5] << 40 |    array[where+status+6] << 48 |  array[where+status+7] << 56 | array[where+status+8] << 64   ;
	 status = status+8;

unsigned char GPR_FPR = array[where+status];
	status++;
			chiara_read_mem_gpr_long(adress,GPR_FPR);

return where+status;

}

int gaspard_scan_opcode (unsigned char opcode,char *array,int whereloop) {
	
	for(int x=0;x<INSTRUCTION_NUM;x++) {
		if(gaspard_opcode_array[x].opcode == opcode) {
			printf("opcode found %d \n ",opcode);
			return gaspard_opcode_array[x].function(array,whereloop);
			}
		}
	printf("opcode not found %d \n",opcode);
	}
