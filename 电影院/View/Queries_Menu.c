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
		printf("************************ 查询信息 *************************\n");
		//printf("[S]演播厅查询.\n");
		printf("[P]剧目查询.\n");
	//printf("[T]票务查询.\n");
		printf("S[c]演出计划查询\n");
		printf("[R]返回.\n");
		printf("\n==================================================================\n");
		printf("请选择:");
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
			printf("请输入要查询的id号:\n");	fflush(stdin);
			scanf("%d",&id);
			
			Schedule_UI_Query(id);
			printf("继续查询  请按enter!退出请按[R]\n");
			choice=getchar();
			break;
		}
	} while ('R' != choice && 'r' != choice);
	}
