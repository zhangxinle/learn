/*
 * Studio_UI.c
 *
 *  Created on: 2015年4月24日
 *      Author: Administrator
 */
#include "Studio_UI.h"

#include "../Common/list.h"
#include "../Service/studio.h"
#include "../Service/seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"

static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>
#include<windows.h>

void Studio_UI_MgtEntry(void) {
	int i, id;
	char choice;

	studio_list_t head;          //双向链表的定义  位于 #include "../Service/studio.h"
	studio_node_t *pos;          //双向链表的一个结点  位于 #include "../Service/studio.h"
	Pagination_t paging;         

	List_Init(head, studio_node_t);        //链表  位于#include "../Common/list.h" 
	paging.offset = 0;       //初始化  记录 
	paging.pageSize = STUDIO_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("********************** 放映室列表 **********************\n");
		printf("%5s  %18s  %10s  %10s  %10s\n", "ID", "Name", "Rows Count",
				"Columns Count", "Seats Count");
		printf("------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (studio_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
			pos = pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf(
				"[P]上一页|[N]下一页  | [A]添加|[D]删除|[U]升级 | [S]座位 | [R]返回");
		printf(
				"\n==================================================================\n");
		printf("请选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("输入 ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("输入 ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) 
			{	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("输入 ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n=======================================================\n");
		printf("****************  添加新的放映室  ****************\n");
		printf("-------------------------------------------------------\n");
		printf("放映室名:");
		fflush(stdin);
		gets(rec.name);
		printf("座椅排数:");
		scanf("%d", &(rec.rowsCount));
		printf("座椅列数:");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = rec.colsCount*rec.rowsCount;
		printf("=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("studio_3");

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("新的放映厅添加成功！\n");
		} else
			printf("新的放映厅添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]继续添加, [R]返回:");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("这个演播厅不能退出!\n请按 [Enter] 退出!\n");
		getchar();
		return 0;
	}

	printf("\n=======================================================\n");
	printf("**************** 升级工程室  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("房间ID:%d\n", rec.id);
	printf("房间名[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
			printf("新的行座位应该为 >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("新的列座位应该为 >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("行座位:");
		scanf("%d", &rec.rowsCount);
		printf("列座位:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount=0;
	}

	printf("-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"房间数据更新成功!\n请按 [Enter] 返回!\n");
	} else
		printf("房间数据更新失败!\n请按 [Enter] 返回!\n");

	getchar();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("房间座位删除失败！\n");
		printf(
				"房间座位删除成功！\n请按 [Enter] 返回!\n");
		rtn = 1;
	} else {
		printf("房间不能退出\n请按 [Enter] 返回!\n");
	}

	getchar();
	return rtn;
}

