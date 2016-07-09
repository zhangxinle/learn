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
	 * Description: 显示所有的演出计划信息,进行查询。
	 * Input:       无
	 * Output:      所有剧目的信息
	 * Return:      无
	 */
void Schedule_UI_ListAll(void) {
	
	int i;

	schedule_list_t head;          //双向链表的定义  
	schedule_node_t *pos;          //双向链表的一个结点  
	Pagination_t paging;         

	List_Init(head, schedule_node_t);        //链表
	paging.offset = 0;       //初始化  记录 
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	system("cls");
		printf("\n=============================================================================\n");
		printf("********************************* 演出计划表 **********************************\n");
		printf("%5s  %18s  %10s  %10s  %10s  %10s\n", "计划ID号", "剧目ID号", "放映厅ID号","演出日期", "演出时间","座位数量");
		printf("-------------------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (schedule_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18d  %10d  %5d年%d月%d日  %5d时%d分%d秒  %10d\n", pos->data.id,
					pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,
					pos->data.time.hour,pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
			pos = pos->next;
		}
		
		printf("------------ 全部记录:%2d ---------------------------- 页数 %2d/%2d ------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"*******************************************************************************\n");
				
}

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: 添加一条演出计划信息
 * Input:       相关剧目的ID号
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Schedule_UI_Add(int play_id) {  
   
	schedule_t  rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n=======================================================\n");
		printf("****************  添加新的演出计划  ****************\n");
		printf("-------------------------------------------------------\n");
		
		printf("剧目ID号:\n");
		fflush(stdin);
		scanf("%d",&(rec.play_id));fflush(stdin);
		
		printf("放映厅ID号：\n"); 
		fflush(stdin);
		scanf("%d",&(rec.studio_id)); fflush(stdin);       
	
		
		printf("演出日期:"); fflush(stdin);
		printf("\n年：");fflush(stdin); scanf("%d",&(rec.date.year));fflush(stdin);
		printf("\n月：");fflush(stdin);scanf("%d",&(rec.date.month));fflush(stdin);
		printf("\n日:"); fflush(stdin); scanf("%d",&rec.date.day);fflush(stdin);
		
		printf("演出时间:"); fflush(stdin);
		printf("\n时：");fflush(stdin); scanf("%d",&(rec.time.hour));fflush(stdin);
		printf("\n分：");fflush(stdin);scanf("%d",&(rec.time.minute));fflush(stdin);
		printf("\n秒:"); fflush(stdin); scanf("%d",&rec.time.second);fflush(stdin);
		
		
		printf("座位数量:\n"); fflush(stdin);
		scanf("%d",&(rec.seat_count));
		fflush(stdin);
		

		printf("=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("schedule");   //分配以一个id 

		if (Schedule_Srv_Add(&rec)) {
			newRecCount ++;
			printf("新的演出计划添加成功！\n");
		} else
			printf("新的演出计划添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]继续添加, [R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: 按照ID号更新演出计划信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Schedule_UI_Modify(int id){
	
	schedule_t rec;
	int choice=0;
	int rtn = 0;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("这个演出计划不能退出!\n请按 [Enter] 退出!\n");
		getchar();
		return 0;
	}
	system("cls");
	printf("\n=======================================================\n");
	printf("**************** 修改演出信息  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("演出计划ID:%d\n", rec.id);
	
	printf("演出剧目ID[%d]改为:", rec.play_id);
	fflush(stdin);
	scanf("%d",&(rec.play_id));fflush(stdin);
	
	printf("演出的放映厅ID[%d]改为:",rec.studio_id);
	fflush(stdin);
	scanf("%d",&(rec.studio_id));fflush(stdin);
	
	printf("演出日期  %d 年 %d 月 %d 日 改为:",rec.date.year,rec.date.month,rec.date.day);
	printf("\n年:");scanf("%d",&(rec.date.year));fflush(stdin);
	printf("\n月:");scanf("%d",&(rec.date.month));fflush(stdin);
	printf("\n日:");scanf("%d",&(rec.date.day ));fflush(stdin);
	
	printf("演出时间 %d 时 %d 分 %d 秒 改为:",rec.time.hour,rec.time.minute,rec.time.second);
	printf("\n时:");scanf("%d",&(rec.time.hour));fflush(stdin);
	printf("\n分:");scanf("%d",&(rec.time.minute));fflush(stdin);
	printf("\n秒:");scanf("%d",&(rec.time.second));fflush(stdin);
	
	printf("座位数量[%d]改为:",rec.seat_count);
	scanf("%d",&(rec.seat_count));fflush(stdin);
	
	printf("-------------------------------------------------------\n");

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"演出数据更新成功!\n请按 [Enter] 返回!\n");
	} else
		printf("演出数据更新失败!\n请按 [Enter] 返回!\n");

	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_UI_Delete(int id){
	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		
		printf("演出计划删除成功！\n");
		rtn = 1;
	} 
	else {
		printf("演出计划不能退出\n请按 [Enter] 返回!\n");
	}

	getchar();
	return rtn;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: 按照ID号查询演出计划信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_UI_Query(int id){
	int rtn = 0;
    schedule_t  buf;
    system("cls");
    if (Schedule_Srv_FetchByID( id, &buf)) {
        rtn = 1;
        printf("=========================== ID为%d的演出计划 ===========================\n",id);
        printf("计划ID: %d\n",buf.id);
        printf("剧目ID: %d\n",buf.play_id);
        printf("放映厅ID: %d\n",buf.studio_id);
        printf("演出日期:%d年%d月%d日\n", buf.date.year, buf.date.month, buf.date.day);
        printf("演出时间:%d时%d分%d秒\n",buf.time.hour, buf.time.minute, buf.time.second);
        printf("座位数量:%d\n",buf.seat_count);
        printf("========================================================================\n");
        printf("输入【enter】结束!!");
        while(getchar() != '\n');
    }
	else    
    {
        printf("查找失败！");
        sleep(1);
    }
    return rtn;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: 以列表模式显示给定剧目的演出计划信息
 * Input:       list为查找到的演出计划信息链表的头指针，play为剧目信息，paging为显示分页设置参数
 * Output:      无
 * Return:      返回查找到的记录数目
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){
	int i=0;
	schedule_node_t *pos;

	paging.totalRecords = Schedule_Srv_FetchAll(list);
	Paging_Locate_FirstPage(list, paging);
	system("cls");
	printf("\n=============================================================================\n");
//	printf("********************************* 与%d有关的演出计划表 **********************************\n",play.id);
	printf("%5s  %18s  %10s  %10s  %10s  %10s\n", "计划ID号", "剧目ID号", "放映厅ID号","演出日期", "演出时间","座位数量");
	printf("-------------------------------------------------------------------------------\n");
	//显示数据
	for (i = 0, pos = (schedule_node_t *) (paging.curPos);
			pos != list && i < paging.pageSize; i++) {
		printf("%5d  %18d  %10d  %5d年%d月%d日  %5d时%d分%d秒  %10d\n", pos->data.id,
				pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,
				pos->data.time.hour,pos->data.time.minute,pos->data.time.second,pos->data.seat_count);
		pos = pos->next;
	}
	
	printf("------------ 全部记录:%2d ---------------------------- 页数 %2d/%2d ------------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
	printf(
			"*******************************************************************************\n");
}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: 演出计划管理界面，演出计划按照剧目进行管理，以剧目ID号为输入
 * Input:       相关剧目ID号
 * Output:      演出计划信息
 * Return:      无
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

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	system("cls");
	do{
		//if(Schedule_Srv_FetchByID( play_id, schedule_t *buf))
		if(!(paging.totalRecords=Schedule_Srv_FetchByPlay( list, play_id)))
		{
			printf("不能获取这个链表！\n");
			return ;
		}
		Schedule_UI_ListByPlay(&play,list,paging);
		printf(" [P].上一页       [A].添加      [U].修改    [D].删除      [Q].查找     [R].返回上一层菜单      [N].下一页\n");
	
		printf("请选择:\n");
		fflush(stdin);
		scanf("%c",&choice); 
		fflush(stdin);
		
		switch(choice)
		{
			case 'a':
			case 'A':
			 	//添加演出计划
			 	if(Schedule_UI_Add(play_id))     //新添加成功，跳到最后一页显示
				{
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, schedule_node_t);
				}
				 break;
			case 'u':
			case 'U':
				 	//修改演出计划
				printf("输入 ID:");
				scanf("%d", &id);
				fflush(stdin);
				if (Schedule_UI_Modify(id)) {	//从新载入数据
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}
				break;
			case 'd':
			case 'D':
				    //删除演出计划
				printf("输入 ID:");
				scanf("%d", &id);
				fflush(stdin);
				if (Schedule_UI_Delete(id)) {	//从新载入数据
					paging.totalRecords = Schedule_Srv_FetchAll(head);
					List_Paging(head, paging, schedule_node_t);
				}	
				break;	
			case 'y':
			case 'Y':
				//打印演出计划
				Schedule_UI_ListAll();
				break;
			case 'q':
			case 'Q':
				printf("输入要查找的ID:");
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
	//释放链表空间
	List_Destroy(head, schedule_node_t);
}

