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
//extern unsigned char CarPlateNum[];//¡¶»ú¶¯³µºÅÅÆºÅÂë£º¡·    ºº×Ö±àÂë
//extern unsigned char CarPlateClass[] ;//¡¶»ú¶¯³µºÅÅÆ·ÖÀà£º¡·    ºº×Ö±àÂë
//extern unsigned char DriversLicenseNum[]  ;//¡¶»ú¶¯³µ¼İÊ»Ö¤ºÅÂë£º¡·    ºº×Ö±àÂë
//extern unsigned char Speed_Status[] ; //¡¶ËÙ¶È×´Ì¬£º¡·    ºº×Ö±àÂë
//extern unsigned char Print_Time[] ;//¡¶´òÓ¡Ê±¼ä£º¡·    ºº×Ö±àÂë
//extern unsigned char OverSpeedRecord[];//¡¶Á½¸öÈÕÀúÌìÄÚ³¬Ê±¼İÊ»¼ÇÂ¼£º¡·    ºº×Ö±àÂë


//extern unsigned char car_plate_province_Jing[]	;/*"¾©",0*/
//extern unsigned char car_plate_province_Jin[]	;/*"½ò",1*/
//extern unsigned char car_plate_province_Hu[]	;/*"»¦",2*/
//extern unsigned char car_plate_province_Yu[]	;/*"Óå",3*/
//extern unsigned char car_plate_province_Ji[]	;/*"¼ª",4*/
//extern unsigned char car_plate_province_Liao[] 	;/*"ÁÉ",5*/
//extern unsigned char car_plate_province_jin[]	;/*"½ú",6*/
//extern unsigned char car_plate_province_ji[]	;/*"¼½",7*/
//extern unsigned char car_plate_province_Qing[] 	;/*"Çà",8*/
//extern unsigned char car_plate_province_Lu[]	;/*"Â³",9*/
//extern unsigned char car_plate_province_Yu[]	;/*"Ô¥",10*/
//extern unsigned char car_plate_province_Su[]	;/*"ËÕ",11*/
//extern unsigned char car_plate_province_Wan[]	;/*"Íî",12*/
//extern unsigned char car_plate_province_Zhe[]	;/*"Õã",13*/
//extern unsigned char car_plate_province_Min[]	;/*"Ãö",14*/
//extern unsigned char car_plate_province_Gan[]	;/*"¸Ó",15*/
//extern unsigned char car_plate_province_Xiang[]	;/*"Ïæ",16*/
//extern unsigned char car_plate_province_E[]	;	/*"¶õ",17*/
//extern unsigned char car_plate_province_Yue[]	;/*"ÔÁ",18*/
//extern unsigned char car_plate_province_Qiong[]	;/*"Çí",19*/
//extern unsigned char car_plate_province_Gan[]	;/*"¸Ê",20*/
//extern unsigned char car_plate_province_Shan[]	;/*"ÉÂ",21*/
//extern unsigned char car_plate_province_Qian[]	;/*"Ç­",22*/
//extern unsigned char car_plate_province_Dian[]	;/*"µá",23*/
//extern unsigned char car_plate_province_Chuan[]	;/*"´¨",24*/
//extern unsigned char car_plate_province_Hei[]	;/*"ºÚ",25*/
//extern unsigned char car_plate_province_Meng[]	;/*"ÃÉ",26*/
//extern unsigned char car_plate_province_Xin[]	;/*"ĞÂ",27*/
//extern unsigned char car_plate_province_Zang[]	;/*"²Ø",28*/
//extern unsigned char car_plate_province_Ning[]	;/*"Äş",29*/
//extern unsigned char car_plate_province_Gui[]	;/*"¹ğ",30*/
//extern unsigned char car_plate_province_Gang[]	;/*"¸Û",31*/
//extern unsigned char car_plate_province_Ao[]	;/*"°Ä",32*/


void Printer_printString(uint8_t* printString);
void print_empty_line(void);
void printChinese(void);


//void printer_info_init(u8 *cmd, u8 *reg_num);

//void printNonChinese(uint8_t* printString,uint16_t printStringLength);

//void printNonChinese(uint8_t* printString);
//void printNonChineseTest(void);
