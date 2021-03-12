/*
Copyright 2021 Elliot-Gaspard COURCHINOUX All rights reserved

---------------The beginning of the  free development license-----------------
This source code is licensed  under the  free development license. An opensource software license. 




See COPYING file to read the free developement license. 

You must agree the license to use this program. 

If you don't agree it, you can't use this software.


---------------The end of free developement license-----------------
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
#include <chiaracompute.h>
#include <tasks.h>
void gaspard_scan_argv_for_dev(int argn,const char *argv[]) {
int status = 0;
while(status!=argn) {

	if(strcmp("-hardrive",argv[status])==0) {
		printf("Ok, hardrive argv detected going to tell hardrive virtual device \n");
		char *nameofhardrive = argv[status+1];
		printf("Name of virtualhardrive %s \n",nameofhardrive);
//~ gaspard_add_hardrivepath(nameofhardrive);
		status++;
		status++;

		} else {
			status++;
			}
	}
	
}



int main (int argn,const char *argv[]) {
if (strcmp(argv[1],"-drivecreate") == 0) {
		int capacity = malloc(100);
printf("please enter capacity in bytes \n");
	int res = scanf("%d",&capacity);
	printf("capacity %d \n",capacity);

	printf("Ok, please enter the name of virtual hardrive : \n");
	char *name = malloc(100);
	int res2 = scanf("%s",name);
int file = open(name,O_WRONLY | O_CREAT, 0644);
unsigned char *stupid8bit = malloc(capacity);
	 write(file,stupid8bit,capacity);
free(name);

return 0;
}
if (strcmp(argv[1],"-kernel") == 0) {


int file = open(argv[2],O_RDONLY);


int end = lseek(file,0,SEEK_END );
	
	lseek(file,0,SEEK_SET  ); // puyt on the begin
	
	// ici tester le fichier ELF ?iso ? raw
unsigned  char  *gaspard = malloc(end);
int size = 	read(file,gaspard,end);
close(file);
int status = 0;
//~ gaspard_scan_opcode(gaspard[status] & masque16(0,5) >>2);
gaspard_alloc_struct();
//~ gaspard_screen_init();
gaspard_scan_argv_for_dev(argn,argv);
basic_instruction_parser(gaspard,size);
printf("fin boucle \n");
while(1);
}else {
	
	printf("Provide elf file using -kernel and the path file \n");
	
	}




}
