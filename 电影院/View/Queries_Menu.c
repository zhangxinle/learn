#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"

void Queries_Menu(void){
	int id;
	char choice;
	do {
		printf("\n==================================================================\n");
		printf("************************ ��ѯ��Ϣ *************************\n");
		//printf("[S]�ݲ�����ѯ.\n");
		printf("[P]��Ŀ��ѯ.\n");
	//printf("[T]Ʊ���ѯ.\n");
		printf("S[c]�ݳ��ƻ���ѯ\n");
		printf("[R]����.\n");
		printf("\n==================================================================\n");
		printf("��ѡ��:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's'://Studio_query;
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(1);
			break;
		case 'T':
		case 't': //Ticket_query;
			break;
		case 'C':
		case 'c':
			printf("������Ҫ��ѯ��id��:\n");	fflush(stdin);
			scanf("%d",&id);
			
			Schedule_UI_Query(id);
			printf("������ѯ  �밴enter!�˳��밴[R]\n");
			choice=getchar();
			break;
		}
	} while ('R' != choice && 'r' != choice);
	}
