/*
 * salesanalysis.c
 *
 *  Created on: 2015��6��12��
 *      Author: Administrator
 */
#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
#include "../Persistence/Sale_Persist.h"
#include "../Persistence/SalesAnalysis_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/list.h"

//����Ա��usrID�ڸ���ʱ����������۶�������۶�
inline int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate){
	
	int amount=0;
	
	sale_list_t saleList;
	sale_list_t pSale;
	
	List_Init(saleList,sale_node_t);//��ʼ������
	Sale_Perst_SelectByUsrID(saleList,usrID,stDate,endDate); //����������Ϣ����������
	
	List_ForEach(saleList,pSale){	//�������� 
		amount+=pSale->data.value;	//�ۼ���Ʊ�� 
	} 
	List_Destroy(saleList,sale_node_t);
	return amount;
}

//��Ͼ�ĿPlay.dat,ͳ���������ݣ��������۷���salesanalysis_list_t list��������list������
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) {
	
	int count=0;
	int sold;	//����Ʊ�� 
	int total;	//��Ʊ�� 
	assert(NULL!=list) ;
	play_list_t playList,pos;
	
	salesanalysis_node_t *newNode;
	

	List_Free(list,salesanalysis_node_t);//�ͷ��������ݽ�� 
	List_Init(playList,play_node_t);//��ʼ�� 
	Play_Srv_FetchAll(playList);//�����Ŀ��Ϣ����������
	
	List_ForEach(playList,pos){ 
		 
		newNode=(salesanalysis_node_t *)malloc(sizeof(salesanalysis_node_t));
		if(!newNode){
			break;
		}
		newNode->data.play_id=pos->data.id;	//��Ŀ��Ϣ 
		newNode->data.start_date=pos->data.start_date;
		newNode->data.end_date=pos->data.end_date;
		
		newNode->data.sales=Schedule_Srv_StatRevByPlay(pos->data.id,&sold,&total);
		newNode->data.totaltickets=sold;
		List_AddTail(list,newNode);
		count++;
	}
	List_Destroy(playList,play_node_t);
	
	return count;
}

//�Ծ�Ժ��ӰƱ������
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list){

	salesanalysis_node_t *p, *listLeft;
	assert(list!=NULL);
	if(List_IsEmpty(list))
		return ;
	list->prev->next = NULL; //��nextָ�빹�ɵ�ѭ����������һ�����Ͽ�
	listLeft = list->next;	    //listLeftָ���һ�����ݽڵ�
	list->next = list->prev = list;    //��list������Ϊ��
	while (listLeft != NULL ) {
		p = list;	//ȡ����һ�����
		listLeft = listLeft->next;
	
	
		Salesanalysis_Srv_AddToSoftedList(list, p);
	
	}

}

void Salesanalysis_Srv_AddToSoftedList(salesanalysis_list_t list, salesanalysis_node_t *node) {

	salesanalysis_node_t *p;
	assert(list!=NULL && node!=NULL);

	if(List_IsEmpty(list))	{
		List_AddTail(list, node);
	}else{
		//Ѱ�Ҳ���λ��
		p=list->next;

		while(p!=list && (p->data.sales>node->data.sales ||
				p->data.sales==node->data.sales )){
			p=p->next;
		}

		//�����node���뵽p֮ǰ
		List_InsertBefore(p, node);
	}
}


