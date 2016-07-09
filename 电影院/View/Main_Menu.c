#include <stdio.h>

#include "Main_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "Queries_Menu.h"
#include "Account_UI.h"
#include "Sale_UI.h"
#include "SalesAnalysis_UI.h"

void Main_Menu(int n) {

	char choice;
	if(n==1)
	{
		do {	
				system("cls");
				printf("\n==================================================================\n");
				printf("**************** 剧院门票管理系统 ****************\n");
				printf("                 [T]售票管理.\n");
				printf("                 [R]退票管理.\n");
				printf("                 [S]返回上一层.\n");
				printf("\n==================================================================\n");
				printf("请选择:");
				fflush(stdin);
				choice = getchar();
				switch (choice) {
				
					case 'T':
					case 't':
						Sale_UI_MgtEntry();
						break;
					case 'R':
					case 'r':
						Sale_UI_ReturnTicket();
						break;
					case 'S':
					case 's':
						SysLogin();
						break;
						 
					}
			} while ('E' != choice && 'e' != choice);
	}
	if(n==2)
	{
		do {
				system("cls");
			printf("\n==================================================================\n");
			printf("**************** 剧院门票管理系统 ****************\n");
			printf("                 [S]演出厅管理.\n");
			printf("                 [P]剧目管理.\n");
			printf("                 [Q]查询管理\n");
			printf("                 [N]分析销售管理.\n");
			printf("                 [D]返回上一层.\n");
			printf("                 [E]退出系统.\n");
			printf("\n==================================================================\n");
			printf("请选择:");
			fflush(stdin);
			choice = getchar();
			switch (choice) {
				case 'S':
				case 's':
					Studio_UI_MgtEntry();
					break;
				case 'P':
				case 'p':
					Play_UI_MgtEntry(0);
					break;
				case 'Q':
				case 'q':
					Queries_Menu();
					break;
			
				case 'N':
				case 'n':
					SalesAanalysis_UI_MgtEntry();
					break;
				case 'D':
				case 'd':
					SysLogin();
					break;
					 
				}
		} while ('E' != choice && 'e' != choice);
	}
}

