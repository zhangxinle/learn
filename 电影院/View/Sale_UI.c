#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../Service/studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���õ�½�û���ȫ�����ݽṹ
extern account_t gl_CurUser;

//���ݼƻ�ID����ʾ�ݳ�Ʊ

void Sale_UI_ShowTicket(int schID){
	schedule_t schRec;
	char choice;
	
	
	seat_list_t seatList;
	ticket_list_t tickList;
	
	if(!Schedule_Srv_FetchByID(schID,&schRec))
	{
		printf("���ID %d �����˳���",schID);
		getchar() ;
		return ;
	}
	
	//������λ��Ϣ
	List_Init(seatList,seat_node_t) ;
	Seat_Srv_FetchByRoomID(seatList,schRec.studio_id);
	
	//����Ʊ����Ϣ
	List_Init(tickList,ticket_node_t) ;
	Ticket_Srv_FetchBySchID(tickList,schID);
	
	do{
		printf("\n==================================================\n");
		Ticket_UI_ListBySch(&schRec,tickList,seatList);    ///????????????????????//
		printf("        [B].��Ʊ                [R].����            ");
		printf("\n==================================================\n");
		printf("��ѡ��\n");fflush(stdin);
		scanf("%c",&choice);
		if(choice=='B'||choice=='b')
		{
			int tickID=Sale_UI_SellTicket(tickList,seatList);
			//��ӡ����Ʊ
			if(tickID) 
			{
				Ticket_UI_Print(tickID);
			}
		}
	
	}while(choice != 'R' && choice != 'r');
	
	List_Destroy(tickList, ticket_node_t);
	List_Destroy(seatList, seat_node_t);
	
}

inline int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList){
	
	int seat_row=0;
	int seat_col=0;//�� 
	int seat_id=0;
	int choice=0;
	//sale_type_t pp;
	ticket_node_t *p=NULL;
	ticket_t rec;
	sale_t sale_rec;
	seat_node_t *pos;
	printf("��������λ���кź��кţ�\n");fflush(stdin);
	scanf("%d%d",&seat_row,&seat_col);
	if(!(pos=Seat_Srv_FindByRowCol(seatList,seat_row,seat_col))) 
	{
	/*	printf("��������Ҫ����Ʊ����λid��\n") ;fflush(stdin);
		scanf("%d",&seat_id);*/ 
		if(!Seat_Srv_FindByID(seatList,pos->data.id))
		{
			p=Ticket_Srv_FindBySeatID(tickList,pos->data.id);
			  Ticket_Srv_FetchByID(pos->data.id, &rec);   //��ȡ�й���λid����Ϣ 
			if(rec.status==0)
			{
				rec.status=1;
			//	p->data.status=1;
			/*	rec.id=p->data.id;
				rec.schedule_id=p->data.schedule_id;
				rec.seat_id=p->data.seat_id;
				rec.price=p->data.price;
				rec.status=1;*/ 
				if(	!Ticket_Srv_Modify(&rec))	
				{	
				
					sale_rec.user_id=gl_CurUser.id ;
					//sale_rec.user_id=123;
					sale_rec.ticket_id=EntKey_Srv_CompNewKey("Sale");
					
					sale_rec.date=DateNow() ;
					sale_rec.date=DateNow();
					
					sale_rec.type=SALE_SELL;
					
					sale_rec.id=EntKey_Srv_CompNewKey("sale") ;
					Sale_Srv_Add(&sale_rec);
					
					return 1;
				}else{
					printf("��ô�����븶Ǯ��\n");
				}
			
			}
			else
			{
				printf("������˼�������ˣ�\n");
			}
		}
		else
		{
			printf("�Բ���û�����Ʊ!\n");
			return 0;
		} 
	}
	else
	{
		printf("�Բ���  û�д���λ��\n");
			return 0;
	}
	
}



//���ݾ�ĿID��ʾ�ݳ��ƻ�
void Sale_UI_ShowScheduler(int playID) {
	
	play_t playRec;
	schedule_list_t head;
	Pagination_t paging;
	
	int schID;
	char choice;
	
	if(!Play_Srv_FetchByID(playID,&playRec)){
		printf("��ĿidΪ%d�ľ�Ŀ�����˳�!������enter����!!\n",playID);
		getchar();
		return ;
	}	
	List_Init(head,schedule_node_t) ;
	paging.totalRecords = Schedule_Srv_FetchByPlay(head,playID);
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	
	
	do{
		printf("\n================================================================================\n");
		Schedule_UI_ListByPlay(&playRec,head,paging);
		printf("[P].��һҳ         [T].��ʾ���е�Ʊ        [N].��һҳ  [R].������һ��\n");fflush(stdin);
		printf("\n===================================================================================\n");
		printf("��ѡ��\n");
		fflush(stdin);
		scanf("%c",&choice);
		
		switch(choice)
		{
			case 't':
			case 'T':
				printf("������Ҫ��ʾ��Ʊ��ID:");fflush(stdin) ;
				scanf("%d",&schID);
				Sale_UI_ShowTicket(schID);
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
	} while(choice!='R' && choice !='r');
 	List_Destroy(head, schedule_node_t);
}

void Sale_UI_MgtEntry() {
	int playID,i;
	char choice;
	char filter[30]={'0'};
	//��ʼ����Ŀ���� 
	play_list_t head;
	Pagination_t paging;         
	          
	List_Init(head, play_node_t);
	paging.offset=0;
	
	paging.pageSize = PLAY_PAGE_SIZE;
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do{
		 Play_UI_ShowList(head,paging) ;
		 printf(" [P].��һҳ           [C].��ʾ�ݳ��ƻ�         [N].��һҳ       [R].�˳�\n");
		 printf("��ѡ��\n");fflush(stdin);
		 scanf("%c",&choice);
		 
		 
		 switch(choice)
		 {
		 	case 'c':
		 	case 'C':
			 		//��ʾ�ݳ��ƻ�
			 		printf("�������ݳ��ƻ���ĿID��");fflush(stdin);
			 		scanf("%d",&playID);
	 				Sale_UI_ShowScheduler(playID);
				break;
			case 'S':
			case 's':
				//��ѯ��Ŀ����
			/*	printf("����������:\n");
				gets(filter) ;
				paging.totalRecords=Play_Srv_FetchByName(head,filter);
				Paging_Locate_FirstPage(head,paging);*/
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
	}while(choice!='R' && choice!='r');
	List_Destroy(head, play_node_t);
}

//��Ʊ
void Sale_UI_ReturnTicket(){
	
	int tickID;
	printf("������Ʊ��ID:\n");
	scanf("%d",&tickID);
	
	
	//����id����
	ticket_t tick;
	if(!Ticket_Srv_FetchByID(tickID,&tick)) {
		printf("�� �����ҵ�%d,  ��enter�˳�!!\n",tickID);
		return;
	}
	else if(tick.status==0)
	{
		printf("������˼  %d��Ʊû�б��۳�������",tickID);
		return;
	}
	tick.status=0;
	Ticket_Srv_Modify(&tick);
	
	//��¼��Ʊ��¼ 
	//���洢
	sale_t sale;
	sale.ticket_id=tickID;
	sale.id=EntKey_Srv_CompNewKey("saleRecord") ;
	sale.user_id=gl_CurUser.id;
//	sale.user_id=123;
	sale.type=SALE_RETURN;
	sale.value=-tick.price;
	//��ȡϵͳ
	sale.date=DateNow() ;
	sale.date=DateNow();
	
	//�������۵�
	 Sale_Srv_Add(&sale);
	
}
