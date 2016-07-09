/*
  * Account_UI.c
 *
 *  Created on: 2015��5��8��
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

//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
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
	printf("�����������������  ��ӭ�������������ӰԺ  ����������������\n\n");
	printf("��                              ��A������Ա��¼                                       ��\n\n");
	printf("��                              ��B����ƱԱ��¼                                       ��\n\n");
	printf("��                              ��M�������¼                                         ��\n\n");
	printf("��                              ��E���˳�                                             ��\n\n");      
//	printf("��                              ��D��������һ��                                       ��\n\n");    
	printf("���������������������������������������������\n\n");
    fflush(stdin);
	choice = getchar();
    switch(choice){
			case 'A':
			case 'a':    //����ϵͳ��ʼ���˺� 
                      while(i<=3)
 				   	  { 
						printf("�������û���:\n");	
     					scanf("%s",username);
	  				    printf("����������:\n");	
     					scanf("%s",password);
						if(Account_Srv_Verify(username,password))
						{
							data=Account_Srv_FindByUsrName(head,username);
							if(data->data.type!=9)
								printf("�˻�Ȩ�޲���\n");
						
						else{
						
 										do {
 												system("cls");
												printf("\n==================================================================\n");
												printf("**************** ��Ժ��Ʊ����ϵͳ ****************\n");
												printf("                 [T]ϵͳ�û�����.\n");
												printf("                 [S]�ݳ�������.\n");
												printf("                 [R]������һ��.\n");
												printf("                 [E]�˳�ϵͳ.\n");
												printf("\n==================================================================\n");
												printf("��ѡ��:");
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
								printf("�������������.\n");
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
					 	printf("�������û���:\n");	
     					scanf("%s",username);
	  				    printf("����������:\n");	
     					scanf("%s",password);
     					if(Account_Srv_Verify(username,password)){
     						data=Account_Srv_FindByUsrName(head,username);
							if(data->data.type!=1)
								printf("�˻�Ȩ�޲���\n");
							else
     						Main_Menu(1);
     					}
     					else
     					{
     						printf("�������������.\n");
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
							  printf("�������û���:\n");	
	     				  	  scanf("%s",username);
		  				      printf("����������:\n");	
	     					  scanf("%s",password);
	     					  if(Account_Srv_Verify(username,password)){
	     					  			data=Account_Srv_FindByUsrName(head,username);
								if(data->data.type!=2)
										printf("�˻�Ȩ�޲���\n");
								else
										Main_Menu(2);
		     					}
	     					  	else
		     					{
		     						printf("�������������.\n");
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

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
void Account_UI_MgtEntry(void) {
	int i=0;
	char choice;
	char username[30];
	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;    
	List_Init(head,account_node_t);
	paging.offset = 0;       //��ʼ��  ��¼ 
	paging.pageSize = ACCOUNT_PAGE_SIZE;
	
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	
	do {
		system("cls");
		printf("\n==================================================================\n");
		printf("********************** ϵͳ�û���Ϣ **********************\n");
		printf("%5s  %10s  %20s  %20s\n", "�û�id", "�û�����", "�û���","�û�����");
		printf("------------------------------------------------------------------\n");
		//��ʾ����
		
		for (i = 0, pos = (account_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("%5d  %10d  %20s  %20s\n", pos->data.id,pos->data.type, pos->data.username, pos->data.password);
			pos = pos->next;
		}
		printf("------- ȫ����¼:%2d ----------------------- ҳ�� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf(
				"******************************************************************\n");
		printf("[P]��һҳ|[N]��һҳ | [A]��� | [D]ɾ�� | [U]�޸� | [R]���� |[Q]��ѯ");
		printf(
				"\n==================================================================\n");
		printf("��ѡ��:");
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
			//��Ӻ���
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


//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list ) {
	int i=0;
	int newRecCount=0;
	char a[30];
	char choice;
	account_t  pos;
	account_node_t *temp;
//	account_list_t p;
	do{
		
		printf("�������û���\n");fflush(stdin);
		gets(pos.username);
		temp=(account_node_t *)malloc(sizeof(account_node_t));
		temp=Account_Srv_FindByUsrName(list,pos.username);
		if(temp!=NULL)
		{
		printf("�û����Ѵ���\n");
		return 0;
		}
		printf("����������\n");
		fflush(stdin);
		scanf("%s",&(pos.password));
		
		printf("�������˻�����:\n");
		printf("0.�����û� 1.���۾��� 2.���� 9.ϵͳ����Ա");
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

//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list ,char usrName[]) {
	account_node_t *temp;
	printf("�������û���\n");
	fflush(stdin);
	scanf("%s",usrName);
	temp=(account_node_t *)malloc(sizeof(account_node_t));
	temp=Account_Srv_FindByUsrName(list,usrName);
	if(temp==NULL)
	{
		printf("�û���������\n");
		return 0;
	}
	else 
	{	printf("����������������\n");
		fflush(stdin);
		scanf("%s",temp->data.password);
		Account_Srv_Modify(&(temp->data));
	printf("�޸ĳɹ�\n");
	return 1;
	
	}
	return 1;
}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_node_t *temp;
	printf("�������û���\n");
	fflush(stdin);
	scanf("%s",usrName);
	temp=(account_node_t *)malloc(sizeof(account_node_t));
	temp=Account_Srv_FindByUsrName(list,usrName);
	if(temp==NULL)
	{
		printf("�û���������\n");
		return 0;
	}
	else if(Account_Srv_DeleteByID(temp->data.id))
	{
	printf("ɾ���ɹ�\n");
	return 1;
	
	}
}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	char choice;
	account_node_t *temp;
	do{
	printf("�������û���\n");
	fflush(stdin);
	scanf("%s",usrName);
	temp=(account_node_t *)malloc(sizeof(account_node_t));
	temp=Account_Srv_FindByUsrName(list,usrName);
	if(temp==NULL)
	{
		printf("�û���������\n");
		return 0;
	}
	else
	{	printf("%5s  %10s  %20s  %20s\n", "�û�id", "�û�����", "�û���","�û�����");
		printf("%5d  %10d  %20s  %20s\n", temp->data.id,temp->data.type, temp->data.username, temp->data.password);
		printf("[Q]ery,[R]eturn");
		fflush(stdin);
		printf("��ѡ��\n");
		scanf("%c", &choice);
	}
	}while ('q' == choice || 'Q' == choice);

}
