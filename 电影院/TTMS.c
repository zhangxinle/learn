/*
 ============================================================================
 Name        : TTMS.c
 Author      : Tangtang
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
//#include <assert.h>
#include "View/Main_Menu.h"


//account_t gl_CurUser={0, USR_ANOMY, "Anonymous",""};

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    
   if(!SysLogin()) {
		printf("\n�Բ�������Ȩ��¼��ϵͳ�밴������˳�......\n");
		getchar();
		exit(0);
	}

   
    
    return EXIT_SUCCESS;
}
