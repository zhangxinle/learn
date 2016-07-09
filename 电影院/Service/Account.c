/*
 * account.c
 *
 *  Created on: 2015��6��12��
 *      Author: Administrator
 */
#include "Account.h"
#include "EntityKey.h"
#include "../Common/list.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>
#include<stdio.h>

//extern account_t gl_CurUser;
account_t gl_CurUser;
//����ϵͳ��ʼ���˺�admin
void Account_Srv_InitSys(){
/*	account_t data_admin;
	printf("���ڴ���ϵͳ��ʼ���˺�admin...\n");
    if(Account_Perst_CheckAccFile()==1)
    {
    	data_admin.id=EntKey_Srv_CompNewKey("account");
    	data_admin.type=USR_ANOMY; 
    	printf("admin�˺ų�ʼ���ɹ���\n"); 
		printf("�ѳ�ʼ����admin�˺�ID�� %d\n",data_admin.id); 
    	printf("��Ϊ�ѳ�ʼ����admin�˺������û���������: \n");
    	scanf("%s%s",data_admin.username,data_admin.password);
	    Account_Srv_Add(&data_admin);
    }

*/ 
}

//��֤��¼�˺��Ƿ��Ѵ��ڣ����ڣ������¼�û���Ϣ��ȫ�ֱ���gl_CurUser��return 1������return 0
inline int Account_Srv_Verify(char usrName[], char pwd[]){
	int n=0;
		  account_t usr;	
		if(Account_Perst_SelByName(usrName,&usr))
		{
			if(!strcmp(pwd,usr.password))
			{	n=1;
				gl_CurUser.id=usr.id;
				strcpy(gl_CurUser.password,usr.password);
				gl_CurUser.type=usr.type;
				strcpy(gl_CurUser.username,usr.username);
			}
		}
	return n;
}


//��֤�û����Ƿ���ڣ�����list�������ڣ����ض�Ӧ���ָ�룻���򣬷���null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) {
	account_node_t *p;
	List_ForEach(list,p)
	if(strcmp(p->data.username,usrName)==0)
		return p;
	return NULL;
}

//���һ���û��˺ţ�ͨ������Account_Perst_Insert(data)����ʵ��
inline int Account_Srv_Add(const account_t *data){
	  return  Account_Perst_Insert(data);
}

//�޸�һ���û��˺ţ�ͨ������Account_Perst_Update(data)����ʵ��
inline int Account_Srv_Modify(const account_t *data){
	return Account_Perst_Update(data);
}

//ɾ��һ���û��˺ţ�ͨ������Account_Perst_DeleteByID(usrID)����ʵ��
inline int Account_Srv_DeleteByID(int usrID){
	if(Account_Perst_DeleteByID(usrID))
	return 1;
}

//��ȡusrID��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelectByID(usrID, buf)����ʵ��
inline int Account_Srv_FetchByID(int usrID, account_t *buf){
	if(Account_Perst_SelectByID(usrID, buf)==1)
	{
		return 1;
	}
	else
	{
		return 0; 
	}

}

//��ȡusrName��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelByName(usrName, buf)����ʵ��
inline int Account_Srv_FetchByName(char usrName[], account_t *buf){
	if(Account_Perst_SelByName(usrName, buf)==1)
	{
		return 1;
	} 
	else{
		return 0;
	}

}

//��ȡ�����û��˺���Ϣ�����浽list�����У�ͨ������Account_Perst_SelectAll(list)����ʵ��
inline int Account_Srv_FetchAll(account_list_t list){
	return Account_Perst_SelectAll(list);
}


