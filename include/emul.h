/*
Copyright 2021 Elliot-Gaspard COURCHINOUX All rights reserved

---------------The beginning of the  free development license-----------------
This source code is licensed  under the  free development license. An opensource software license. 




See COPYING file to read the free developement license. 

You must agree the license to use this program. 

If you don't agree it, you can't use this software.


---------------The end of free developement license-----------------
*/
 
 
struct pagination_general_configuration{
unsigned short page_table_num:16;

unsigned char num_of_page_child:8;

unsigned char child_size:8;

unsigned long long adress_of_page:64;
	
	
	
	
}; 
 
 struct pagination_general_configuration *pag_config;
struct page_child {
	
unsigned char is_writed:1;
unsigned char mode:4;

unsigned char disabled_param:4;		
	
unsigned long long adress;	
	
}; 


struct page_table{
	
unsigned char mode:4;

unsigned char disabled:4;

unsigned char present:1;

struct page_child child[];	
	
};
 

struct interrupt {
unsigned char active:1;

unsigned long long adress;	
	
	
};
struct interrupt *chiara64_interrupt;

struct gaspard_opcode{
	unsigned char opcode;
	
	int (*function)();
	
	};
	int gaspard_disp(unsigned char*array,int where);
	int gaspard_dispa(unsigned char*array,int where);
	int gaspard_add(unsigned char*array,int where);
	int gaspard_inc(unsigned char*array,int where);
	int gaspard_sub(unsigned char*array,int where);
	int gaspard_mult(unsigned char*array,int where);
	int gaspard_xor(unsigned char*array,int where);
	int gaspard_and(unsigned char*array,int where);
	int gaspard_or(unsigned char*array,int where);
	int gaspard_not(unsigned char*array,int where);
	int gaspard_shl(unsigned char*array,int where);
	int gaspard_shr(unsigned char*array,int where);
	int gaspard_cmp(unsigned char*array,int where);
	int gaspard_config(unsigned char*array,int where);
	int gaspard_decrement(unsigned char*array,int where);
	int gaspard_prcfg(unsigned char*array,int where);
	int gaspard_func(unsigned char*array,int where);
	int gaspard_calle(unsigned char*array,int where);
	int gaspard_callz(unsigned char*array,int where);
	int gaspard_callc(unsigned char*array,int where);
	int gaspard_callo(unsigned char*array,int where);
	int gaspard_callrz(unsigned char*array,int where);
	int gaspard_callrc(unsigned char*array,int where);
	int gaspard_callro(unsigned char*array,int where);
	int gaspard_modul(unsigned char*array,int where);
	int gaspard_ADDC(unsigned char*array,int where);
	int gaspard_SUBC(unsigned char*array,int where);
	int gaspard_div(unsigned char*array,int where);
	int gaspard_divs(unsigned char*array,int where);
	int gaspard_dispGPRmem(unsigned char*array,int where);
	int gaspard_dispIMMmem(unsigned char*array,int where);
	int gaspard_POP(unsigned char*array,int where);
	int gaspard_PUSH(unsigned char*array,int where);
	int gaspard_ret(unsigned char*array,int where);
	int gaspard_syscall(unsigned char*array,int where);
	int gaspard_dispab_read(unsigned char*array,int where); 
	int gaspard_dispab_write(unsigned char*array,int where); 
	int gaspard_dispas_read(unsigned char*array,int where);
	int gaspard_dispas_write(unsigned char*array,int where);
	int gaspard_dispal_read(unsigned char*array,int where);
	int gaspard_dispal_write(unsigned char*array,int where);

#define INSTRUCTION_NUM 40
	
	static struct gaspard_opcode gaspard_opcode_array[40] = {
		{.opcode=0x3,.function=gaspard_dispa},
		{.opcode=44,.function=gaspard_disp},
		{.opcode=46,.function=gaspard_add},
		{.opcode=0x4,.function=gaspard_inc},
		{.opcode=0x5,.function=gaspard_sub},
		{.opcode=0x7,.function=gaspard_mult},
		{.opcode=0x8,.function=gaspard_xor},
		{.opcode=0x9,.function=gaspard_and},
		{.opcode=0xa,.function=gaspard_or},
		{.opcode=0xb,.function=gaspard_not},
		{.opcode=0xc,.function=gaspard_shl},
		{.opcode=0xd,.function=gaspard_shr},
		{.opcode=0xf,.function=gaspard_cmp},
		{.opcode=45,.function=gaspard_config},
		{.opcode=34,.function=gaspard_decrement},
		{.opcode=150,.function=gaspard_prcfg},
		{.opcode=17,.function=gaspard_func},
		{.opcode=18,.function=gaspard_calle},
		{.opcode=19,.function=gaspard_callz},
		{.opcode=20,.function=gaspard_callc},
		{.opcode=21,.function=gaspard_callo},
		{.opcode=22,.function=gaspard_callrz},
		{.opcode=23,.function=gaspard_callrc},
		{.opcode=24,.function=gaspard_callro},
		{.opcode=25,.function=gaspard_modul},
		{.opcode=26,.function=gaspard_ADDC},
		{.opcode=27,.function=gaspard_SUBC},
		{.opcode=28,.function=gaspard_div},
		{.opcode=29,.function=gaspard_divs},
		{.opcode=30,.function=gaspard_dispIMMmem},
		{.opcode=31,.function=gaspard_dispGPRmem},
		{.opcode=32,.function=gaspard_ret},
		{.opcode=33,.function=gaspard_POP},
		{.opcode=35,.function=gaspard_PUSH},
		{.opcode=36,.function=gaspard_syscall},
		{.opcode=25,.function=gaspard_dispab_read},
		{.opcode=26,.function=gaspard_dispab_write},
		{.opcode=40,.function=gaspard_dispas_read},
		{.opcode=41,.function=gaspard_dispas_write},
		{.opcode=45,.function=gaspard_dispal_read},
		{.opcode=46,.function=gaspard_dispal_write},
	};
	// 110 is reserved
static unsigned short masque16(int debut,int fin) {
	 int bitsactive =fin-debut;
	 	 int bitsmettreazero =15-bitsactive;

	 unsigned short final ;
	for (int x=0;x<bitsactive+1;x++){
//~ final |= x<<1;
final|= 1 << x;
 }
 


	// printf("bitsmettreazero %x \n",final);
//	 printf("bitsmettreazero %x \n",final<<fin-bitsactive);
	 //~ return final<<fin-bitsactive;
	 return final<<(15-fin)-1;
}
static int bitExtracted(int number, int k, int p) 
{ 
    return (((1 << k)) & (number >> (p))); 
} 
