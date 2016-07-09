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
				printf("**************** ��Ժ��Ʊ����ϵͳ ****************\n");
				printf("                 [T]��Ʊ����.\n");
				printf("                 [R]��Ʊ����.\n");
				printf("                 [S]������һ��.\n");
				printf("\n==================================================================\n");
				printf("��ѡ��:");
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
			printf("**************** ��Ժ��Ʊ����ϵͳ ****************\n");
			printf("                 [S]�ݳ�������.\n");
			printf("                 [P]��Ŀ����.\n");
			printf("                 [Q]��ѯ����\n");
			printf("                 [N]�������۹���.\n");
			printf("                 [D]������һ��.\n");
			printf("                 [E]�˳�ϵͳ.\n");
			printf("\n==================================================================\n");
			printf("��ѡ��:");
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

