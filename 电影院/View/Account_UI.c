/*
  * Account_UI.c
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */

#include "Account_UI.h"
#include "../Common/list.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include<windows.h>
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>

//登录函数，提示用户输入用户名和密码，登录成功return 1，否则提示重新登录，超过3次，登录失败
int SysLogin() {

	int i=1,j,k;
   	account_list_t data,head;//
   	List_Init(head,account_node_t);//
   	data=(account_list_t)malloc(sizeof(account_node_t));//
    Account_Srv_FetchAll(head);//
    char choice;
    char username[30];
	char password[30];
	char vipname[10]={"vip"};
	char vippassword[10]={"123456"};
	do{
			system("cls");
	printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆  欢迎来到博纳新天地影院  ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n\n");
	printf("☆                              【A】管理员登录                                       ☆\n\n");
	printf("☆                              【B】售票员登录                                       ☆\n\n");
	printf("☆                              【M】经理登录                                         ☆\n\n");
	printf("☆                              【E】退出                                             ☆\n\n");      
//	printf("☆                              【D】返回上一层                                       ☆\n\n");    
	printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n\n");
    fflush(stdin);
	choice = getchar();
    switch(choice){
			case 'A':
			case 'a':    //创建系统初始化账号 
                      while(i<=3)
 				   	  { 
						printf("请输入用户名:\n");	
     					scanf("%s",username);
	  				    printf("请输入密码:\n");	
     					scanf("%s",password);
						if(Account_Srv_Verify(username,password))
						{
							data=Account_Srv_FindByUsrName(head,username);
							if(data->data.type!=9)
								printf("账户权限不够\n");
						
						else{
						
 										do {
 												system("cls");
												printf("\n==================================================================\n");
												printf("**************** 剧院门票管理系统 ****************\n");
												printf("                 [T]系统用户管理.\n");
												printf("                 [S]演出厅管理.\n");
												printf("                 [R]返回上一层.\n");
												printf("                 [E]退出系统.\n");
												printf("\n==================================================================\n");
												printf("请选择:");
												fflush(stdin);
												choice = getchar();
												switch (choice) {
												
													case 'T':
													case 't':
														Account_UI_MgtEntry();
														break;
													case 'S':
													case 's':
														Studio_UI_MgtEntry();
														break;	
													case 'R':
													case 'r':
														SysLogin(); 
														break;
												
													}
											} while ('E' != choice && 'e' != choice);
											break; 
 								}
 						
					}
						else 
						{
								printf("密码错误，请重试.\n");
								j=0; 
								i++;
								fflush(stdin); 
								continue;
						}
				  	  }
				  	  if(j==0)
					  {
							exit(0);
					  }		
					  break;
		     	case 'B':
				case 'b': 
					 while(i<=3)  {
					 	printf("请输入用户名:\n");	
     					scanf("%s",username);
	  				    printf("请输入密码:\n");	
     					scanf("%s",password);
     					if(Account_Srv_Verify(username,password)){
     						data=Account_Srv_FindByUsrName(head,username);
							if(data->data.type!=1)
								printf("账户权限不够\n");
							else
     						Main_Menu(1);
     					}
     					else
     					{
     						printf("密码错误，请重试.\n");
							j=0; 
							i++;
							fflush(stdin); 
							continue;
     					}
     					if(j==0)
     					{
							exit(0);
     					}
					 }
     					break;	
     			case 'M':
				case 'm': while(i<=3) 
						{
							  printf("请输入用户名:\n");	
	     				  	  scanf("%s",username);
		  				      printf("请输入密码:\n");	
	     					  scanf("%s",password);
	     					  if(Account_Srv_Verify(username,password)){
	     					  			data=Account_Srv_FindByUsrName(head,username);
								if(data->data.type!=2)
										printf("账户权限不够\n");
								else
										Main_Menu(2);
		     					}
	     					  	else
		     					{
		     						printf("密码错误，请重试.\n");
									j=0; 
									i++;
									fflush(stdin); 
									continue;
		     					}
		     					if(j==0)
		     					{
									exit(0);
		     					}		
						  }
					
     					break;	
			    case 'E':
			    case 'e':return 0;
			    case 'D':
				case 'd':
						SysLogin(); 
						break; 
			    
            }}while(1);


}

//管理系统用户功能入口函数，显示用户账号管理菜单
void Account_UI_MgtEntry(void) {
	int i=0;
	char choice;
	char username[30];
	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;    
	List_Init(head,account_node_t);
	paging.offset = 0;       //初始化  记录 
	paging.pageSize = ACCOUNT_PAGE_SIZE;
	
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	
	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("********************** 系统用户信息 **********************\n");
		printf("%5s  %10s  %20s  %20s\n", "用户id", "用户类型", "用户名","用户密码");
		printf("------------------------------------------------------------------\n");
		//显示数据
		
		for (i = 0, pos = (account_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %10d  %20s  %20s\n", pos->data.id,pos->data.type, pos->data.username, pos->data.password);
			pos = pos->next;
		}
		printf("------- 全部记录:%2d ----------------------- 页数 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf("[P]上一页|[N]下一页 | [A]添加 | [D]删除 | [U]修改 | [R]返回 |[Q]查询");
		printf(
				"\n==================================================================\n");
		printf("请选择:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
		switch(choice)
		{
			case 'A':
			case 'a':
				if(	Account_UI_Add(head) )
				{
					paging.totalRecords = Account_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, account_node_t);
				}
				break;
			//添加函数
			case 'D':
			case 'd':
				if(Account_UI_Delete(head,username))
				{
					paging.totalRecords = Account_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, account_node_t);
				}
				break;
			case 'U':
			case 'u':
				if(Account_UI_Modify(head,username)) 
					{
					paging.totalRecords = Account_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, account_node_t);
					}
				break;
			case 'p':
			case 'P':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
				}
				break;	 
			case 'q':
			case 'Q':
				Account_UI_Query(head,username);
			/*if(Account_UI_Query(head,username));
				{
					paging.totalRecords = Account_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, account_node_t);
				} */
				break;
		}

	}while(choice!='r'&&choice!='R');


}


//添加一个用户账号信息，如果账号名存在，提示出错信息
int Account_UI_Add(account_list_t list ) {
	int i=0;
	int newRecCount=0;
	char a[30];
	char choice;
	account_t  pos;
	account_node_t *temp;
//	account_list_t p;
	do{
		
		printf("请输入用户名\n");fflush(stdin);
		gets(pos.username);
		temp=(account_node_t *)malloc(sizeof(account_node_t));
		temp=Account_Srv_FindByUsrName(list,pos.username);
		if(temp!=NULL)
		{
		printf("用户名已存在\n");
		return 0;
		}
		printf("请输入密码\n");
		fflush(stdin);
		scanf("%s",&(pos.password));
		
		printf("请输入账户类型:\n");
		printf("0.匿名用户 1.销售经理 2.经理 9.系统管理员");
		fflush(stdin);
		scanf("%d",&i);
		pos.type=(account_type_t)i;
		pos.id=EntKey_Srv_CompNewKey("usr");
				
	
	if (Account_Srv_Add(&pos)) {
		newRecCount ++;
		printf("The new room added successfully!\n");
	//	p=(account_list_t)malloc(sizeof(account_node_t));
	//	p->data=pos;
	//	List_AddTail(list,p);
		
	} else
		printf("The new room added failed!\n");
		
	printf("-------------------------------------------------------\n");
	printf("[A]dd more, [R]eturn:");
	fflush(stdin);
	scanf("%c", &choice);
} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(account_list_t list ,char usrName[]) {
	account_node_t *temp;
	printf("请输入用户名\n");
	fflush(stdin);
	scanf("%s",usrName);
	temp=(account_node_t *)malloc(sizeof(account_node_t));
	temp=Account_Srv_FindByUsrName(list,usrName);
	if(temp==NULL)
	{
		printf("用户名不存在\n");
		return 0;
	}
	else 
	{	printf("请输入您的新密码\n");
		fflush(stdin);
		scanf("%s",temp->data.password);
		Account_Srv_Modify(&(temp->data));
	printf("修改成功\n");
	return 1;
	
	}
	return 1;
}

//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_node_t *temp;
	printf("请输入用户名\n");
	fflush(stdin);
	scanf("%s",usrName);
	temp=(account_node_t *)malloc(sizeof(account_node_t));
	temp=Account_Srv_FindByUsrName(list,usrName);
	if(temp==NULL)
	{
		printf("用户名不存在\n");
		return 0;
	}
	else if(Account_Srv_DeleteByID(temp->data.id))
	{
	printf("删除成功\n");
	return 1;
	
	}
}

//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	char choice;
	account_node_t *temp;
	do{
	printf("请输入用户名\n");
	fflush(stdin);
	scanf("%s",usrName);
	temp=(account_node_t *)malloc(sizeof(account_node_t));
	temp=Account_Srv_FindByUsrName(list,usrName);
	if(temp==NULL)
	{
		printf("用户名不存在\n");
		return 0;
	}
	else
	{	printf("%5s  %10s  %20s  %20s\n", "用户id", "用户类型", "用户名","用户密码");
		printf("%5d  %10d  %20s  %20s\n", temp->data.id,temp->data.type, temp->data.username, temp->data.password);
		printf("[Q]ery,[R]eturn");
		fflush(stdin);
		printf("请选择\n");
		scanf("%c", &choice);
	}
	}while ('q' == choice || 'Q' == choice);

}
