#include "Sale_Persist.h"
#include "../Common/list.h"
#include "../Service/Sale.h"
#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>


static const char SALE_DATA_FILE[] = "Sale.dat";
static const char SALE_DATA_TEMP_FILE[] = "SaleTmp.dat";

int Sale_Perst_Insert(const sale_t *data) {
	
	assert(NULL!=data);
	FILE *fp = fopen(SALE_DATA_FILE, "ab");
	int rtn = 0;
	if (NULL == fp) {
		printf("��������ļ� %s!\n", SALE_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(sale_t), 1, fp);

	fclose(fp);
	return rtn;
}


int Sale_Perst_DeleteByID(int saleID) {
		//��ԭʼ�����ļ�������
	if(rename(SALE_DATA_FILE, SALE_DATA_TEMP_FILE)<0){
		printf("���ܴ� %s�ļ�!\n", SALE_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SALE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour ){
		printf("���ܴ� %s�ļ�!\n", SALE_DATA_FILE);
		return 0;
	}

	fpTarg = fopen(SALE_DATA_FILE, "wb");
	if ( NULL == fpTarg ) {
		printf("���ܴ� %s�ļ�!\n", SALE_DATA_TEMP_FILE);
		return 0;
	}


	sale_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(sale_t), 1, fpSour)) {
			if (saleID == buf.id) {
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(sale_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(SALE_DATA_TEMP_FILE);
	return found;

}

//�����û�ID�������ʱ�������ڵ����ۼ�¼
int Sale_Perst_SelectByUsrID(sale_list_t list, int usrID, user_date_t stDate,
		user_date_t endDate) {

	
	return 0;
}


//���ݸ���ʱ�������ڵ����ۼ�¼
int Sale_Perst_SelectByDate(sale_list_t list, user_date_t stDate,
		user_date_t endDate) {

	
	return 0;
}

/*

//queryUid = null,query all the uid Sale information return the all uid Sale information number
//queryUid =  uid, query the uid Sale information return the uid Sale  number
 *
int  Sale_Infor_By_Uid(char * queryUid, Sale_list_t head)
{

	return 0;
}

int Sale_Infor_By_Time(char  queryTime[][30], Sale_list_t head)
{

	
	return 0;
}

//����Ʊ��id���Ʊ�Ļ�����Ϣ 
int Select_Price_By_Playid(int id, ticket_t *buf)
{
	
		return 0;
}
*/

