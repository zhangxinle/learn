/*
* File name:  Play_UI.c
* File ID:	  TTMS_SSF_Play_UI_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/list.h"
#include "../Service/play.h"
#include "../Service/EntityKey.h"


//static const int PLAY_PAGE_SIZE = 5;

#include <stdio.h>
#include <stdlib.h>
#include<windows.h>

	/*
	 * Function:    Play_UI_ShowList
	 * Function ID:	TTMS_SCU_Play_UI_Show
	 * Description: ���б�ģʽ��ʾ��Ŀ��Ϣ
	 * Input:       list��Ŀ��Ϣ�����ͷָ�룬paging��ҳ���ò���
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ���ز��ҵ��ļ�¼��Ŀ
	 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	
	int i,id;
	char chioce ;
	
	
	play_node_t *pos; 
	
//	List_Init(list, Play_list_t);
	//�������� 
	
	
	paging.totalRecords = Play_Srv_FetchAll(list);
	Paging_Locate_FirstPage(list, paging);
	
	system("cls");
		printf("\n=========================================================================================================\n");
		printf("***************************************************** ��Ӱ�б� *****************************************************\n");
		printf("%5s  %18s  %10s   %10s   %10s  %10s   %10s  %10s   %10s\n", "��ϢID��", "��Ŀ����", "��Ŀ����",
				"��Ŀ��Դ����", "��Ŀ����","�ݳ�ʱ��","��ӳ����","��������","Ʊ�ļ۸�");
		printf("--------------------------------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (play_node_t *) (paging.curPos);
				pos != list && i < paging.pageSize; i++) {
			printf("%5d  %18s  %10d  %10s  %10d   %10d  %5d��%d��%d��  %5dʱ%d��%d��  %10d\n", pos->data.id,
					pos->data.name, pos->data.type, pos->data.area,pos->data.rating,pos->data.duration,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day,
					pos->data.price);
			pos = pos->next;
		}
		printf("---------- ȫ����¼:%2d ----------------------------------------------------------------------- ҳ�� %2d/%2d --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"********************************************************************************************************************\n");
}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: ��Ŀ��Ϣ�������
 * Input:       flagΪ0���������ҳ�棬flagΪ1�����ѯ����
 * Output:      ��Ŀ�������
 * Return:      ��
 */
void Play_UI_MgtEntry(int flag){
	int i, id;
	char choice;

	play_list_t head;          
	play_node_t *pos;          
	Pagination_t paging;         

	List_Init(head, play_node_t);        
	paging.offset = 0;       
	paging.pageSize = PLAY_PAGE_SIZE;

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	if(!flag)
	{
			system("cls");
		do{
			printf("\n\n                                   ��ӭ�����Ŀ��Ϣ�������                                          \n");
			Play_UI_ShowList(head,paging);
			printf(
					"[P]��һҳ|         |         [A]��� | [D]ɾ�� | [U]���� | [M]�ݳ��ƻ� | [R]����           |          [N]��һҳ \n");
		
			printf("��ѡ��\n");
			fflush(stdin);
			scanf("%c",&choice);
			fflush(stdin);
		
			switch(choice)
			{
				case 'a':
				case 'A':
					if(Play_UI_Add()) 
					{
						paging.totalRecords = Play_Srv_FetchAll(head);
						Paging_Locate_LastPage(head, paging, play_node_t);
					}
					break;
				case 'U':
				case 'u':
					//�޸� 
					printf("���� ID:");
					scanf("%d", &id);
					if (Play_UI_Modify(id)) 
					{	//������������
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'd':
				case 'D':
					//ɾ��
					printf("���� ID:");
					scanf("%d", &id);
					if (Play_UI_Delete(id)) {	    //������������
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'm':
				case 'M':
					//�ݳ��ƻ�
					printf("���� ID:");fflush(stdin);
					scanf("%d",&id);
					Schedule_UI_MgtEntry(id);
					break;
				case 'p':
				case 'P':
					if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
					}
					break;
				case 'n':
				case 'N':
					if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
					}
					break;	
			}
		}while(choice!='r' && choice !='R');
	}
	if(flag)
	{
			printf("����Ҫ��Ѱ�� ID:");fflush(stdin);
			scanf("%d",&id);
			Play_UI_Query(id);
			printf("������ѯ�밴enter\n");
			choice=getchar();
			
	}
	//�ͷ�����ռ�
	List_Destroy(head, play_node_t);
}


/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: ���һ����Ŀ��Ϣ
 * Input:       ��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Play_UI_Add(void)      //��Ŀ��Ӻ��� 
{
	play_t rec;
	int choice=0,year=0,month=0,day=0;
	int newRecCount = 0;

	do {
		system("cls");
		printf("\n=======================================================\n");
		printf("****************  ����µľ�Ŀ  ****************\n");
		printf("-------------------------------------------------------\n");
		
		
		printf("��Ŀ����:\n");
		fflush(stdin);
		gets(rec.name);
		
		printf("��Ŀ���ͣ�\n");         //ö������   �Ƿ�ѡ�� 
		printf("1��ʾ��Ӱ��2��ʾ��磬3��ʾ���ֻ�\n");
		printf("��ѡ��\n");
		scanf("%d",&choice);
		fflush(stdin);
		rec.type=(play_type_t)choice;
		
		printf("��Ŀ��Դ����:\n");
		gets(rec.area);
		fflush(stdin);
		
		printf("��Ŀ����:\n") ;
		printf("1��ʾ��ͯ�ɹۿ���2��ʾ������ɹۿ���3��ʾ���˿ɹۿ�\n");
		printf("��ѡ��\n");
		scanf("%d",&choice);
		fflush(stdin);
		rec.rating=(play_rating_t)choice;
		
		printf("�ݳ�ʱ��:\n");
		scanf("%d",&(rec.duration));
		fflush(stdin);
		
		printf("��ӳ����:\n");       //�Ƿ����������ȷ�� 
		printf("��ݣ�\n");
		scanf("%d",&year);
			fflush(stdin);
		printf("�·ݣ�\n") ;
		scanf("%d",&month);
			fflush(stdin);
		printf("����:\n");
		scanf("%d",&day);
			fflush(stdin);
		rec.start_date.year=year;
		rec.start_date.month=month;
		rec.start_date.day=day;
		fflush(stdin);
		
		printf("��������:\n");            //�Ƿ����������ȷ��
			printf("��ݣ�\n");
		scanf("%d",&year);
			fflush(stdin);
		printf("�·ݣ�\n") ;
		scanf("%d",&month);
			fflush(stdin);
		printf("����:\n");
		scanf("%d",&day);
			fflush(stdin);
		rec.end_date.year=year;
		rec.end_date.month=month;
		rec.end_date.day=day;
		fflush(stdin);
		
		printf("Ʊ�ļ۸�\n");
		scanf("%d",&(rec.price));
		fflush(stdin);
		

		printf("=======================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("play");
		
		if (Play_Srv_Add(&rec)) {
			newRecCount ++;
			printf("�µľ�Ŀ��ӳɹ���\n");
		} else
			printf("�µľ�Ŀ���ʧ��!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]�������, [R]����:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: ���¾�Ŀ��Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_UI_Modify(int id){
	play_t rec;
	int choice=0;
	int rtn = 0;
	//seat_list_t list;
   //	int seatcount;

	//Load record
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("�����Ӱ�����˳�!\n�밴 [Enter] �˳�!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("**************** �޸ľ�Ŀ��Ϣ  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("��ĿID:%d\n", rec.id);
	
	printf("��Ŀ��[%s]��Ϊ:", rec.name);
	fflush(stdin);
	gets(rec.name);
	
	printf("��Ŀ���͸�Ϊ��\n");
	printf("1.��Ӱ��2.��磬3.���ֻ�\n"); 
	fflush(stdin);
	scanf("%d",&choice);
	rec.type=(play_type_t)choice;
	
	printf("��Ŀ��Դ����[%s]��Ϊ��",rec.area);
	fflush(stdin);
	gets(rec.area);
	
	printf("��Ŀ����[%s]��Ϊ:\n",rec.rating);
	printf("1.��ͯ�ɹۿ���2.������ɹۿ���3.���˿ɹۿ�\n");
	fflush(stdin);
	scanf("%d",&choice);
	rec.rating=(play_rating_t)choice;
	
	printf("��Ŀ��ӳʱ��%d  %d  %d��Ϊ��",rec.start_date.year,rec.start_date.month,rec.start_date.day);
	printf("�꣺");scanf("%d",&(rec.start_date.year)) ;fflush(stdin);
	printf("�£�");scanf("%d",&(rec.start_date.month));fflush(stdin);
	printf("��: ");scanf("%d",&(rec.start_date.day));fflush(stdin);
	
	printf("��Ŀ����ʱ��%d  %d  %d��Ϊ��",rec.end_date.year,rec.end_date.month,rec.end_date.day);
	printf("�꣺");scanf("%d",&(rec.end_date.year)) ;fflush(stdin);
	printf("�£�");scanf("%d",&(rec.end_date.month));fflush(stdin);
	printf("��: ");scanf("%d",&(rec.end_date.day));fflush(stdin);
	
	printf("��ĿƱ��[%d]��Ϊ��",rec.price);
	fflush(stdin);
	scanf("%d",&rec.price);


	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"��Ŀ���ݸ��³ɹ�!\n�밴 [Enter] ����!\n");
	} else
		printf("��Ŀ���ݸ���ʧ��!\n�밴 [Enter] ����!\n");

	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: ����ID��ɾ����Ŀ��Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Play_UI_Delete(int id){
	
	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		
		printf("��Ŀɾ���ɹ���\n");
		rtn = 1;
	} 
	else {
		printf("��Ŀ�����˳�\n�밴 [Enter] ����!\n");
	}

	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: ����ID�Ų�ѯ��Ŀ��Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_UI_Query(int id){
	int i=0;
	play_t buf;
	if(Play_Srv_FetchByID(id,&buf))
	{
		i=1;
		printf("\n=========================================================================================================\n");
		printf("***************************************************** ��Ӱ�б� *****************************************************\n");
		printf("%5s  %18s  %10s   %10s   %10s  %10s   %10s  %10s   %10s\n", "��ϢID��", "��Ŀ����", "��Ŀ����",
				"��Ŀ��Դ����", "��Ŀ����","�ݳ�ʱ��","��ӳ����","��������","Ʊ�ļ۸�");
		printf("--------------------------------------------------------------------------------------------\n");
		
		printf("%5d  %18s  %10d  %10s  %10d   %10d  %5d��%d��%d��  %5dʱ%d��%d��  %10d\n", buf.id,
				buf.name, buf.type, buf.area,buf.rating,buf.duration,
				buf.start_date.year,buf.start_date.month,buf.start_date.day,
				buf.end_date.year,buf.end_date.month,buf.end_date.day,
				buf.price);
		printf(
			"********************************************************************************************************************\n");			
	}
	return i;
}
