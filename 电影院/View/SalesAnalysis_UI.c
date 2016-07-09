/*
 * SalesAnalysis_UI.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */ 
#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include <conio.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//�ⲿ����gl_CurUser ���ڴ洢��¼�û���Ϣ

extern account_t gl_CurUser;

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
void SalesAnalysis_UI_BoxOffice() {
	int i;
	char choice;
	
	salesanalysis_list_t head;          
	salesanalysis_node_t *pos;          
	Pagination_t paging;         

	List_Init(head, salesanalysis_node_t);        
	paging.offset = 0;       
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	//��������
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);   //����Ʊ������
	
	Paging_Locate_FirstPage(head, paging);
	do{
		printf("=========================================================================\n");
		printf("****************************Ʊ��ͼ��*****************************\n");
		printf("-------------------------------------------------------------------------\n");
		printf("��Ŀ��\t\t����\t\t��Ʊ��\tƱ��\t��ӳʱ��\t\t��ӳʱ��\n");
		
			for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%-10s\t%-10s\t%-5ld\t%-5ld\t%d-%d-%d\t%d-%d-%d\t\n",
					pos->data.name, pos->data.area, pos->data.totaltickets,pos->data.sales,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
		}
		printf("---------- ȫ����¼:%2d --------------------------- ҳ�� %2d/%2d --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"********************************************************************************************************************\n");
		printf("[P].��һҳ                                                                       [N].��һҳ                [R].����\n");
		fflush(stdin);
		scanf("%c",&choice);
		
		switch(choice)
		{
			case 'P':
			case 'p':
					if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
					}
				break;
			case 'N':
			case 'n':
					if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
					}
				break;			
		}
	} while(choice!='R' && choice!='r');
		List_Destroy(head, salesanalysis_node_t);
}

//��ʾ��ƱԱ�ڸ��������������Ʊ���
void SalesAnalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){    //���ڲ������û����뻹�ǣ� 
	account_t usr;
	int sale_count=0;
	sale_count=Salesanalysis_Srv_CompSaleVal(usrID,stDate,endDate);    //��ȡ����Ա�����۶�   
	
	if(!Account_Srv_FetchByID(usrID,&usr)) {
		printf("������˼��û��Ū��!\n�밴enter����\n");
		getchar();
		return ;
	}
	
	printf("================��ƱԱ%d���������===================\n",usrID);
	printf("���ۿ�ʼ����:     %d-%d-%d\n",stDate.year,stDate.month,stDate.day);
	printf("���۽�������:     %d-%d-%d\n",endDate.year,endDate.month,endDate.day);
	printf("���۶          %d\n",sale_count);
	printf("======================================================\n");
	
}

//���۷�����ں�������ʾ�˵����˵�����"������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�"������ʾ���ѯ������ƱԱ��Ʊ�������
//����ѯ������������ĳ��ƱԱ��Ʊ�����
void SalesAanalysis_UI_MgtEntry() {
	
	user_date_t stDate,endDate;
	account_t user; 
	char choice;
	do{
		printf("���������������Ӧ�ַ���\n\n");
		printf("===============================================\n");
		printf("      [B]��ĿƱ�����а�\n");
		printf("      [D]��ѯԱ���ڸ��������������۶����\n");
		printf("      [S]��ѯԱ���ڸ�������������Ʊ���\n");
		printf("===============================================\n");
		printf("���������������Ӧ�ַ���\n\n");
		fflush(stdin);
		scanf("%c",&choice);
		switch(choice)
		{
			case 'B':
			case 'b':
					SalesAnalysis_UI_BoxOffice();
				break;
			case 'D':
			case 'd':
				if(gl_CurUser.type==USR_CLERK)
					SalesAnalysis_UI_StatSale(gl_CurUser.id,stDate,endDate);//���� 
				else
 					{
 						printf("��������ƱԱ������\n");
 						fflush(stdin);
 						char UsrName[100];
						scanf("%s",UsrName);		
						if(!Account_Srv_FetchByName(UsrName,&user))
							printf("�����ڸ���ƱԱ��Ϣ\n");
						else
							SalesAnalysis_UI_StatSale(user.id,stDate,endDate);//����
					}
				break;
			case 'S':
			case 's':
				if(gl_CurUser.type==USR_MANG)
				{
					char UsrName[100];
					printf("��������ƱԱ������\n");
					scanf("%s",UsrName);
						fflush(stdin);
					if(!Account_Srv_FetchByName(UsrName,&user))
						printf("�����ڸ���ƱԱ��Ϣ\n");
					else
						{
							printf("�������Ŀ��ӳʱ��:\n");	fflush(stdin);
							scanf("%d-%d-%d",&stDate.year,&stDate.month,&stDate.day);
							printf("�������Ŀ��ӳʱ��:\n");	fflush(stdin);
							scanf("%d-%d-%d",&endDate.year,&endDate.month,&endDate.day);
							SalesAnalysis_UI_StatSale(user.id,stDate,endDate);
						}
				}
				else
					{
						printf("û��Ȩ��"); 
						break;
					}
		 } 
	}while(choice!='R' && choice!='r' );
}



