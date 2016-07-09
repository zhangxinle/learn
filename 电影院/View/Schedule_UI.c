/*
* File name:  Schedule_UI.c
* File ID:	  TTMS_SSF_Schedule_UI_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/


#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/list.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"


//static const int SCHEDULE_PAGE_SIZE = 5;

#include <stdio.h>
#include <stdlib.h>
#include<windows.h>

	/*
	 * Function:    Schedule_UI_ListAll
	 * Function ID:	TTMS_SCU_Schedule_UI_List
	 * Description: ��ʾ���е��ݳ��ƻ���Ϣ,���в�ѯ��
	 * Input:       ��
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ��
	 */
void Schedule_UI_ListAll(void) {
	
	int i;

	schedule_list_t head;          //˫������Ķ���  
	schedule_node_t *pos;          //˫�������һ�����  
	Pagination_t paging;         

	List_Init(head, schedule_node_t);        //����
	paging.offset = 0;       //��ʼ��  ��¼ 
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//��������
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	system("cls");
		printf("\n=============================================================================\n");
		printf("********************************* �ݳ��ƻ��� **********************************\n");
		printf("%5s  %18s  %10s  %10s  %10s  %10s\n", "�ƻ�ID��", "��ĿID��", "��ӳ��ID��","�ݳ�����", "�ݳ�ʱ��","��λ����");
		printf("-------------------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18d  %10d  %5d��%d��%d��  %5dʱ%d��%d��  %10d\n", pos->data.id,
					pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,
					pos->data.time.hour,pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;
		}
		
		printf("------------ ȫ����¼:%2d ---------------------------- ҳ�� %2d/%2d ------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"*******************************************************************************\n");
				
}

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ��ؾ�Ŀ��ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Schedule_UI_Add(int play_id) {  
   
	schedule_t  rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n=======================================================\n");
		printf("****************  ����µ��ݳ��ƻ�  ****************\n");
		printf("-------------------------------------------------------\n");
		
		printf("��ĿID��:\n");
		fflush(stdin);
		scanf("%d",&(rec.play_id));fflush(stdin);
		
		printf("��ӳ��ID�ţ�\n"); 
		fflush(stdin);
		scanf("%d",&(rec.studio_id)); fflush(stdin);       
	
		
		printf("�ݳ�����:"); fflush(stdin);
		printf("\n�꣺");fflush(stdin); scanf("%d",&(rec.date.year));fflush(stdin);
		printf("\n�£�");fflush(stdin);scanf("%d",&(rec.date.month));fflush(stdin);
		printf("\n��:"); fflush(stdin); scanf("%d",&rec.date.day);fflush(stdin);
		
		printf("�ݳ�ʱ��:"); fflush(stdin);
		printf("\nʱ��");fflush(stdin); scanf("%d",&(rec.time.hour));fflush(stdin);
		printf("\n�֣�");fflush(stdin);scanf("%d",&(rec.time.minute));fflush(stdin);
		printf("\n��:"); fflush(stdin); scanf("%d",&rec.time.second);fflush(stdin);
		
		
		printf("��λ����:\n"); fflush(stdin);
		scanf("%d",&(rec.seat_count));
		fflush(stdin);
		

		printf("=======================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("schedule");   //������һ��id 

		if (Schedule_Srv_Add(&rec)) {
			newRecCount ++;
			printf("�µ��ݳ��ƻ���ӳɹ���\n");
		} else
			printf("�µ��ݳ��ƻ����ʧ��!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]�������, [R]����:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: ����ID�Ÿ����ݳ��ƻ���Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_UI_Modify(int id){
	
	schedule_t rec;
	int choice=0;
	int rtn = 0;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("����ݳ��ƻ������˳�!\n�밴 [Enter] �˳�!\n");
		getchar();
		return 0;
	}
	system("cls");
	printf("\n=======================================================\n");
	printf("**************** �޸��ݳ���Ϣ  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("�ݳ��ƻ�ID:%d\n", rec.id);
	
	printf("�ݳ���ĿID[%d]��Ϊ:", rec.play_id);
	fflush(stdin);
	scanf("%d",&(rec.play_id));fflush(stdin);
	
	printf("�ݳ��ķ�ӳ��ID[%d]��Ϊ:",rec.studio_id);
	fflush(stdin);
	scanf("%d",&(rec.studio_id));fflush(stdin);
	
	printf("�ݳ�����  %d �� %d �� %d �� ��Ϊ:",rec.date.year,rec.date.month,rec.date.day);
	printf("\n��:");scanf("%d",&(rec.date.year));fflush(stdin);
	printf("\n��:");scanf("%d",&(rec.date.month));fflush(stdin);
	printf("\n��:");scanf("%d",&(rec.date.day ));fflush(stdin);
	
	printf("�ݳ�ʱ�� %d ʱ %d �� %d �� ��Ϊ:",rec.time.hour,rec.time.minute,rec.time.second);
	printf("\nʱ:");scanf("%d",&(rec.time.hour));fflush(stdin);
	printf("\n��:");scanf("%d",&(rec.time.minute));fflush(stdin);
	printf("\n��:");scanf("%d",&(rec.time.second));fflush(stdin);
	
	printf("��λ����[%d]��Ϊ:",rec.seat_count);
	scanf("%d",&(rec.seat_count));fflush(stdin);
	
	printf("-------------------------------------------------------\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"�ݳ����ݸ��³ɹ�!\n�밴 [Enter] ����!\n");
	} else
		printf("�ݳ����ݸ���ʧ��!\n�밴 [Enter] ����!\n");

	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_UI_Delete(int id){
	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		
		printf("�ݳ��ƻ�ɾ���ɹ���\n");
		rtn = 1;
	} 
	else {
		printf("�ݳ��ƻ������˳�\n�밴 [Enter] ����!\n");
	}

	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: ����ID�Ų�ѯ�ݳ��ƻ���Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_UI_Query(int id){
	int rtn = 0;
    schedule_t  buf;
    system("cls");
    if (Schedule_Srv_FetchByID( id, &buf)) {
        rtn = 1;
        printf("=========================== IDΪ%d���ݳ��ƻ� ===========================\n",id);
        printf("�ƻ�ID: %d\n",buf.id);
        printf("��ĿID: %d\n",buf.play_id);
        printf("��ӳ��ID: %d\n",buf.studio_id);
        printf("�ݳ�����:%d��%d��%d��\n", buf.date.year, buf.date.month, buf.date.day);
        printf("�ݳ�ʱ��:%dʱ%d��%d��\n",buf.time.hour, buf.time.minute, buf.time.second);
        printf("��λ����:%d\n",buf.seat_count);
        printf("========================================================================\n");
        printf("���롾enter������!!");
        while(getchar() != '\n');
    }
	else    
    {
        printf("����ʧ�ܣ�");
        sleep(1);
    }
    return rtn;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: ���б�ģʽ��ʾ������Ŀ���ݳ��ƻ���Ϣ
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬playΪ��Ŀ��Ϣ��pagingΪ��ʾ��ҳ���ò���
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){
	int i=0;
	schedule_node_t *pos;

	paging.totalRecords = Schedule_Srv_FetchAll(list);
	Paging_Locate_FirstPage(list, paging);
	system("cls");
	printf("\n=============================================================================\n");
//	printf("********************************* ��%d�йص��ݳ��ƻ��� **********************************\n",play.id);
	printf("%5s  %18s  %10s  %10s  %10s  %10s\n", "�ƻ�ID��", "��ĿID��", "��ӳ��ID��","�ݳ�����", "�ݳ�ʱ��","��λ����");
	printf("-------------------------------------------------------------------------------\n");
	//��ʾ����
	for (i = 0, pos = (schedule_node_t *) (paging.curPos);
			pos != list && i < paging.pageSize; i++) {
		printf("%5d  %18d  %10d  %5d��%d��%d��  %5dʱ%d��%d��  %10d\n", pos->data.id,
				pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,
				pos->data.time.hour,pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
		pos = pos->next;
	}
	
	printf("------------ ȫ����¼:%2d ---------------------------- ҳ�� %2d/%2d ------------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
	printf(
			"*******************************************************************************\n");
}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: �ݳ��ƻ�������棬�ݳ��ƻ����վ�Ŀ���й����Ծ�ĿID��Ϊ����
 * Input:       ��ؾ�ĿID��
 * Output:      �ݳ��ƻ���Ϣ
 * Return:      ��
 */
void Schedule_UI_MgtEntry(int play_id){
	int id;
	char choice;
	
	play_t play;
	
	schedule_list_t head; 
	schedule_list_t list;         
	schedule_node_t *pos;          
	Pagination_t paging; 
	        
	Play_Srv_FetchByID(play_id,&play);
	
	List_Init(head, schedule_node_t);        
	paging.offset = 0;       
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//��������
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	system("cls");
	do{
		//if(Schedule_Srv_FetchByID( play_id, schedule_t *buf))
		if(!(paging.totalRecords=Schedule_Srv_FetchByPlay( list, play_id)))
		{
			printf("���ܻ�ȡ�������\n");
			return ;
		}
		Schedule_UI_ListByPlay(&play,list,paging);
		printf(" [P].��һҳ       [A].���      [U].�޸�    [D].ɾ��      [Q].����     [R].������һ��˵�      [N].��һҳ\n");
	
		printf("��ѡ��:\n");
		fflush(stdin);
		scanf("%c",&choice); 
		fflush(stdin);
		
		switch(choice)
		{
			case 'a':
			case 'A':
			 	//����ݳ��ƻ�
			 	if(Schedule_UI_Add(play_id))     //����ӳɹ����������һҳ��ʾ
				{
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, schedule_node_t);
				}
				 break;
			case 'u':
			case 'U':
				 	//�޸��ݳ��ƻ�
				printf("���� ID:");
				scanf("%d", &id);
				fflush(stdin);
				if (Schedule_UI_Modify(id)) {	//������������
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}
				break;
			case 'd':
			case 'D':
				    //ɾ���ݳ��ƻ�
				printf("���� ID:");
				scanf("%d", &id);
				fflush(stdin);
				if (Schedule_UI_Delete(id)) {	//������������
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}	
				break;	
			case 'y':
			case 'Y':
				//��ӡ�ݳ��ƻ�
				Schedule_UI_ListAll();
				break;
			case 'q':
			case 'Q':
				printf("����Ҫ���ҵ�ID:");
				scanf("%d",&id) ;
				fflush(stdin);
				Schedule_UI_Query(id);
				break;
			case 'p':
			case 'P':
				if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
				}
				break;
		}
	}while(choice!='R' && choice!='r');
	//�ͷ�����ռ�
	List_Destroy(head, schedule_node_t);
}

