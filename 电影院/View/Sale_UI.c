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

//引用登陆用户的全局数据结构
extern account_t gl_CurUser;

//根据计划ID，显示演出票

void Sale_UI_ShowTicket(int schID){
	schedule_t schRec;
	char choice;
	
	
	seat_list_t seatList;
	ticket_list_t tickList;
	
	if(!Schedule_Srv_FetchByID(schID,&schRec))
	{
		printf("这个ID %d 不能退出！",schID);
		getchar() ;
		return ;
	}
	
	//载入座位信息
	List_Init(seatList,seat_node_t) ;
	Seat_Srv_FetchByRoomID(seatList,schRec.studio_id);
	
	//载入票的信息
	List_Init(tickList,ticket_node_t) ;
	Ticket_Srv_FetchBySchID(tickList,schID);
	
	do{
		printf("\n==================================================\n");
		Ticket_UI_ListBySch(&schRec,tickList,seatList);    ///????????????????????//
		printf("        [B].买票                [R].返回            ");
		printf("\n==================================================\n");
		printf("请选择：\n");fflush(stdin);
		scanf("%c",&choice);
		if(choice=='B'||choice=='b')
		{
			int tickID=Sale_UI_SellTicket(tickList,seatList);
			//打印销售票
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
	int seat_col=0;//列 
	int seat_id=0;
	int choice=0;
	//sale_type_t pp;
	ticket_node_t *p=NULL;
	ticket_t rec;
	sale_t sale_rec;
	seat_node_t *pos;
	printf("请输入座位的行号和列号：\n");fflush(stdin);
	scanf("%d%d",&seat_row,&seat_col);
	if(!(pos=Seat_Srv_FindByRowCol(seatList,seat_row,seat_col))) 
	{
	/*	printf("请输入你要购买票的座位id：\n") ;fflush(stdin);
		scanf("%d",&seat_id);*/ 
		if(!Seat_Srv_FindByID(seatList,pos->data.id))
		{
			p=Ticket_Srv_FindBySeatID(tickList,pos->data.id);
			  Ticket_Srv_FetchByID(pos->data.id, &rec);   //获取有关座位id的信息 
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
					printf("怎么？！想付钱？\n");
				}
			
			}
			else
			{
				printf("不好意思！卖完了！\n");
			}
		}
		else
		{
			printf("对不起！没有这个票!\n");
			return 0;
		} 
	}
	else
	{
		printf("对不起！  没有此座位！\n");
			return 0;
	}
	
}



//根据剧目ID显示演出计划
void Sale_UI_ShowScheduler(int playID) {
	
	play_t playRec;
	schedule_list_t head;
	Pagination_t paging;
	
	int schID;
	char choice;
	
	if(!Play_Srv_FetchByID(playID,&playRec)){
		printf("剧目id为%d的剧目不能退出!请输入enter结束!!\n",playID);
		getchar();
		return ;
	}	
	List_Init(head,schedule_node_t) ;
	paging.totalRecords = Schedule_Srv_FetchByPlay(head,playID);
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	
	
	do{
		printf("\n================================================================================\n");
		Schedule_UI_ListByPlay(&playRec,head,paging);
		printf("[P].上一页         [T].显示所有的票        [N].下一页  [R].返回上一层\n");fflush(stdin);
		printf("\n===================================================================================\n");
		printf("请选择：\n");
		fflush(stdin);
		scanf("%c",&choice);
		
		switch(choice)
		{
			case 't':
			case 'T':
				printf("请输入要显示的票的ID:");fflush(stdin) ;
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
	//初始化剧目链表 
	play_list_t head;
	Pagination_t paging;         
	          
	List_Init(head, play_node_t);
	paging.offset=0;
	
	paging.pageSize = PLAY_PAGE_SIZE;
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do{
		 Play_UI_ShowList(head,paging) ;
		 printf(" [P].上一页           [C].显示演出计划         [N].下一页       [R].退出\n");
		 printf("请选择：\n");fflush(stdin);
		 scanf("%c",&choice);
		 
		 
		 switch(choice)
		 {
		 	case 'c':
		 	case 'C':
			 		//显示演出计划
			 		printf("请输入演出计划剧目ID：");fflush(stdin);
			 		scanf("%d",&playID);
	 				Sale_UI_ShowScheduler(playID);
				break;
			case 'S':
			case 's':
				//查询剧目名字
			/*	printf("请输入名字:\n");
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

//退票
void Sale_UI_ReturnTicket(){
	
	int tickID;
	printf("请输入票的ID:\n");
	scanf("%d",&tickID);
	
	
	//根据id查找
	ticket_t tick;
	if(!Ticket_Srv_FetchByID(tickID,&tick)) {
		printf("娃 不能找到%d,  按enter退出!!\n",tickID);
		return;
	}
	else if(tick.status==0)
	{
		printf("不好意思  %d号票没有被售出！！！",tickID);
		return;
	}
	tick.status=0;
	Ticket_Srv_Modify(&tick);
	
	//记录退票记录 
	//并存储
	sale_t sale;
	sale.ticket_id=tickID;
	sale.id=EntKey_Srv_CompNewKey("saleRecord") ;
	sale.user_id=gl_CurUser.id;
//	sale.user_id=123;
	sale.type=SALE_RETURN;
	sale.value=-tick.price;
	//获取系统
	sale.date=DateNow() ;
	sale.date=DateNow();
	
	//保存销售单
	 Sale_Srv_Add(&sale);
	
}
