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
	 * Description: 以列表模式显示剧目信息
	 * Input:       list剧目信息链表的头指针，paging分页设置参数
	 * Output:      所有剧目的信息
	 * Return:      返回查找到的记录数目
	 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	
	int i,id;
	char chioce ;
	
	
	play_node_t *pos; 
	
//	List_Init(list, Play_list_t);
	//载入数据 
	
	
	paging.totalRecords = Play_Srv_FetchAll(list);
	Paging_Locate_FirstPage(list, paging);
	
	system("cls");
		printf("\n=========================================================================================================\n");
		printf("***************************************************** 电影列表 *****************************************************\n");
		printf("%5s  %18s  %10s   %10s   %10s  %10s   %10s  %10s   %10s\n", "信息ID号", "剧目名称", "剧目类型",
				"剧目来源地区", "剧目级别","演出时长","上映日期","结束日期","票的价格");
		printf("--------------------------------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (play_node_t *) (paging.curPos);
				pos != list && i < paging.pageSize; i++) {
			printf("%5d  %18s  %10d  %10s  %10d   %10d  %5d年%d月%d日  %5d时%d分%d秒  %10d\n", pos->data.id,
					pos->data.name, pos->data.type, pos->data.area,pos->data.rating,pos->data.duration,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day,
					pos->data.price);
			pos = pos->next;
		}
		printf("---------- 全部记录:%2d ----------------------------------------------------------------------- 页数 %2d/%2d --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"********************************************************************************************************************\n");
}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: 剧目信息管理界面
 * Input:       flag为0，进入管理页面，flag为1进入查询界面
 * Output:      剧目管理界面
 * Return:      无
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

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	if(!flag)
	{
			system("cls");
		do{
			printf("\n\n                                   欢迎进入剧目信息管理界面                                          \n");
			Play_UI_ShowList(head,paging);
			printf(
					"[P]上一页|         |         [A]添加 | [D]删除 | [U]升级 | [M]演出计划 | [R]返回           |          [N]下一页 \n");
		
			printf("请选择：\n");
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
					//修改 
					printf("输入 ID:");
					scanf("%d", &id);
					if (Play_UI_Modify(id)) 
					{	//从新载入数据
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'd':
				case 'D':
					//删除
					printf("输入 ID:");
					scanf("%d", &id);
					if (Play_UI_Delete(id)) {	    //从新载入数据
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 'm':
				case 'M':
					//演出计划
					printf("输入 ID:");fflush(stdin);
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
			printf("输入要查寻的 ID:");fflush(stdin);
			scanf("%d",&id);
			Play_UI_Query(id);
			printf("继续查询请按enter\n");
			choice=getchar();
			
	}
	//释放链表空间
	List_Destroy(head, play_node_t);
}


/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: 添加一条剧目信息
 * Input:       无
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Play_UI_Add(void)      //剧目添加函数 
{
	play_t rec;
	int choice=0,year=0,month=0,day=0;
	int newRecCount = 0;

	do {
		system("cls");
		printf("\n=======================================================\n");
		printf("****************  添加新的剧目  ****************\n");
		printf("-------------------------------------------------------\n");
		
		
		printf("剧目名称:\n");
		fflush(stdin);
		gets(rec.name);
		
		printf("剧目类型：\n");         //枚举类型   是否选择？ 
		printf("1表示电影，2表示歌剧，3表示音乐会\n");
		printf("请选择：\n");
		scanf("%d",&choice);
		fflush(stdin);
		rec.type=(play_type_t)choice;
		
		printf("剧目来源地区:\n");
		gets(rec.area);
		fflush(stdin);
		
		printf("剧目级别:\n") ;
		printf("1表示儿童可观看，2表示青少年可观看，3表示成人可观看\n");
		printf("请选择：\n");
		scanf("%d",&choice);
		fflush(stdin);
		rec.rating=(play_rating_t)choice;
		
		printf("演出时长:\n");
		scanf("%d",&(rec.duration));
		fflush(stdin);
		
		printf("上映日期:\n");       //是否控制输入正确？ 
		printf("年份：\n");
		scanf("%d",&year);
			fflush(stdin);
		printf("月份：\n") ;
		scanf("%d",&month);
			fflush(stdin);
		printf("日期:\n");
		scanf("%d",&day);
			fflush(stdin);
		rec.start_date.year=year;
		rec.start_date.month=month;
		rec.start_date.day=day;
		fflush(stdin);
		
		printf("结束日期:\n");            //是否控制输入正确？
			printf("年份：\n");
		scanf("%d",&year);
			fflush(stdin);
		printf("月份：\n") ;
		scanf("%d",&month);
			fflush(stdin);
		printf("日期:\n");
		scanf("%d",&day);
			fflush(stdin);
		rec.end_date.year=year;
		rec.end_date.month=month;
		rec.end_date.day=day;
		fflush(stdin);
		
		printf("票的价格\n");
		scanf("%d",&(rec.price));
		fflush(stdin);
		

		printf("=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("play");
		
		if (Play_Srv_Add(&rec)) {
			newRecCount ++;
			printf("新的剧目添加成功！\n");
		} else
			printf("新的剧目添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]继续添加, [R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: 更新剧目信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_UI_Modify(int id){
	play_t rec;
	int choice=0;
	int rtn = 0;
	//seat_list_t list;
   //	int seatcount;

	//Load record
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("这个电影不能退出!\n请按 [Enter] 退出!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("**************** 修改剧目信息  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("剧目ID:%d\n", rec.id);
	
	printf("剧目名[%s]改为:", rec.name);
	fflush(stdin);
	gets(rec.name);
	
	printf("剧目类型改为：\n");
	printf("1.电影，2.歌剧，3.音乐会\n"); 
	fflush(stdin);
	scanf("%d",&choice);
	rec.type=(play_type_t)choice;
	
	printf("剧目来源地区[%s]改为：",rec.area);
	fflush(stdin);
	gets(rec.area);
	
	printf("剧目级别[%s]改为:\n",rec.rating);
	printf("1.儿童可观看，2.青少年可观看，3.成人可观看\n");
	fflush(stdin);
	scanf("%d",&choice);
	rec.rating=(play_rating_t)choice;
	
	printf("剧目上映时间%d  %d  %d改为：",rec.start_date.year,rec.start_date.month,rec.start_date.day);
	printf("年：");scanf("%d",&(rec.start_date.year)) ;fflush(stdin);
	printf("月：");scanf("%d",&(rec.start_date.month));fflush(stdin);
	printf("日: ");scanf("%d",&(rec.start_date.day));fflush(stdin);
	
	printf("剧目下线时间%d  %d  %d改为：",rec.end_date.year,rec.end_date.month,rec.end_date.day);
	printf("年：");scanf("%d",&(rec.end_date.year)) ;fflush(stdin);
	printf("月：");scanf("%d",&(rec.end_date.month));fflush(stdin);
	printf("日: ");scanf("%d",&(rec.end_date.day));fflush(stdin);
	
	printf("剧目票价[%d]改为：",rec.price);
	fflush(stdin);
	scanf("%d",&rec.price);


	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"剧目数据更新成功!\n请按 [Enter] 返回!\n");
	} else
		printf("剧目数据更新失败!\n请按 [Enter] 返回!\n");

	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: 按照ID号删除剧目信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_UI_Delete(int id){
	
	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		
		printf("剧目删除成功！\n");
		rtn = 1;
	} 
	else {
		printf("剧目不能退出\n请按 [Enter] 返回!\n");
	}

	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: 按照ID号查询剧目信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Play_UI_Query(int id){
	int i=0;
	play_t buf;
	if(Play_Srv_FetchByID(id,&buf))
	{
		i=1;
		printf("\n=========================================================================================================\n");
		printf("***************************************************** 电影列表 *****************************************************\n");
		printf("%5s  %18s  %10s   %10s   %10s  %10s   %10s  %10s   %10s\n", "信息ID号", "剧目名称", "剧目类型",
				"剧目来源地区", "剧目级别","演出时长","上映日期","结束日期","票的价格");
		printf("--------------------------------------------------------------------------------------------\n");
		
		printf("%5d  %18s  %10d  %10s  %10d   %10d  %5d年%d月%d日  %5d时%d分%d秒  %10d\n", buf.id,
				buf.name, buf.type, buf.area,buf.rating,buf.duration,
				buf.start_date.year,buf.start_date.month,buf.start_date.day,
				buf.end_date.year,buf.end_date.month,buf.end_date.day,
				buf.price);
		printf(
			"********************************************************************************************************************\n");			
	}
	return i;
}
