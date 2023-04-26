#include "sys.h"



#pragma pack(push)
#pragma pack(1)
typedef struct{
	unsigned char reg_num_chinese[12];
	unsigned char reg_num[19];	
	unsigned char set_chinese[];
}printer_info_t;

#pragma pack()

extern unsigned char out_line[];


//extern unsigned char set_chinese[];
//extern unsigned char CarPlateNum[];//�����������ƺ��룺��    ���ֱ���
//extern unsigned char CarPlateClass[] ;//�����������Ʒ��ࣺ��    ���ֱ���
//extern unsigned char DriversLicenseNum[]  ;//����������ʻ֤���룺��    ���ֱ���
//extern unsigned char Speed_Status[] ; //���ٶ�״̬����    ���ֱ���
//extern unsigned char Print_Time[] ;//����ӡʱ�䣺��    ���ֱ���
//extern unsigned char OverSpeedRecord[];//�������������ڳ�ʱ��ʻ��¼����    ���ֱ���


//extern unsigned char car_plate_province_Jing[]	;/*"��",0*/
//extern unsigned char car_plate_province_Jin[]	;/*"��",1*/
//extern unsigned char car_plate_province_Hu[]	;/*"��",2*/
//extern unsigned char car_plate_province_Yu[]	;/*"��",3*/
//extern unsigned char car_plate_province_Ji[]	;/*"��",4*/
//extern unsigned char car_plate_province_Liao[] 	;/*"��",5*/
//extern unsigned char car_plate_province_jin[]	;/*"��",6*/
//extern unsigned char car_plate_province_ji[]	;/*"��",7*/
//extern unsigned char car_plate_province_Qing[] 	;/*"��",8*/
//extern unsigned char car_plate_province_Lu[]	;/*"³",9*/
//extern unsigned char car_plate_province_Yu[]	;/*"ԥ",10*/
//extern unsigned char car_plate_province_Su[]	;/*"��",11*/
//extern unsigned char car_plate_province_Wan[]	;/*"��",12*/
//extern unsigned char car_plate_province_Zhe[]	;/*"��",13*/
//extern unsigned char car_plate_province_Min[]	;/*"��",14*/
//extern unsigned char car_plate_province_Gan[]	;/*"��",15*/
//extern unsigned char car_plate_province_Xiang[]	;/*"��",16*/
//extern unsigned char car_plate_province_E[]	;	/*"��",17*/
//extern unsigned char car_plate_province_Yue[]	;/*"��",18*/
//extern unsigned char car_plate_province_Qiong[]	;/*"��",19*/
//extern unsigned char car_plate_province_Gan[]	;/*"��",20*/
//extern unsigned char car_plate_province_Shan[]	;/*"��",21*/
//extern unsigned char car_plate_province_Qian[]	;/*"ǭ",22*/
//extern unsigned char car_plate_province_Dian[]	;/*"��",23*/
//extern unsigned char car_plate_province_Chuan[]	;/*"��",24*/
//extern unsigned char car_plate_province_Hei[]	;/*"��",25*/
//extern unsigned char car_plate_province_Meng[]	;/*"��",26*/
//extern unsigned char car_plate_province_Xin[]	;/*"��",27*/
//extern unsigned char car_plate_province_Zang[]	;/*"��",28*/
//extern unsigned char car_plate_province_Ning[]	;/*"��",29*/
//extern unsigned char car_plate_province_Gui[]	;/*"��",30*/
//extern unsigned char car_plate_province_Gang[]	;/*"��",31*/
//extern unsigned char car_plate_province_Ao[]	;/*"��",32*/


void Printer_printString(uint8_t* printString);
void print_empty_line(void);
void printChinese(void);


//void printer_info_init(u8 *cmd, u8 *reg_num);

//void printNonChinese(uint8_t* printString,uint16_t printStringLength);

//void printNonChinese(uint8_t* printString);
//void printNonChineseTest(void);
