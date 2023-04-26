#include "printercmd.h"
#include "terminal_parameter.h"
#include "printer.h"
#include <string.h>
#include "usart.h"


unsigned char out_line[] = {0x1b,0x66,0x01,0x01};//Êä³öÁ½¸ö¿ÕÐÐ
u8 printer_cmd[200];

//unsigned char set_chinese[] = {0x1c,0x26};
//unsigned char CarPlateNum[] = {0xBB,0xFA, 0xB6,0xAF, 0xB3,0xB5, 0xBA,0xC5, 0xC5,0xC6, 0xBA,0xC5, 0xC2,0xEB, 0xA3,0xBA};//¡¶»ú¶¯³µºÅÅÆºÅÂë£º¡·    ºº×Ö±àÂë
//unsigned char CarPlateClass[] = {0xBB,0xFA, 0xB6,0xAF, 0xB3,0xB5, 0xBA,0xC5, 0xC5,0xC6, 0xB7,0xD6, 0xC0,0xE0, 0xA3,0xBA};//¡¶»ú¶¯³µºÅÅÆ·ÖÀà£º¡·    ºº×Ö±àÂë
//unsigned char DriversLicenseNum[] = {0xBB,0xFA, 0xB6,0xAF, 0xB3,0xB5, 0xBC,0xDD, 0xCA,0xBB, 0xD6,0xA4, 0xBA,0xC5, 0xC2,0xEB, 0xA3,0xBA};//¡¶»ú¶¯³µ¼ÝÊ»Ö¤ºÅÂë£º¡·    ºº×Ö±àÂë
//unsigned char Speed_Status[] = {0xCB,0xD9, 0xB6,0xC8, 0xD7,0xB4, 0xCC,0xAC, 0xA3,0xBA}; //¡¶ËÙ¶È×´Ì¬£º¡·    ºº×Ö±àÂë
//unsigned char Print_Time[] = {0xB4,0xF2,  0xD3,0xA1,  0xCA,0xB1,  0xBC,0xE4, 0xA3,0xBA}; //¡¶´òÓ¡Ê±¼ä£º¡·    ºº×Ö±àÂë
//unsigned char OverSpeedRecord[] = {0xC1,0xBD, 0xB8,0xF6, 0xC8,0xD5, 0xC0,0xFA, 0xCC,0xEC, 0xC4,0xDA, 0xB3,0xAC, 0xCA,0xB1, 0xBC,0xDD, 0xCA,0xBB, 0xBC,0xC7, 0xC2,0xBC, 0xA3,0xBA};//¡¶Á½¸öÈÕÀúÌìÄÚ³¬Ê±¼ÝÊ»¼ÇÂ¼£º¡·    ºº×Ö±àÂë

//	
//unsigned char car_plate_province_Jing[]	= {0xBE,0xA9};/*"¾©",0*/
//unsigned char car_plate_province_Jin[]	= {0xBD,0xF2};/*"½ò",1*/
//unsigned char car_plate_province_Hu[]	= {0xBB,0xA6};/*"»¦",2*/
//unsigned char car_plate_province_Yu[]	= {0xD3,0xE5};/*"Óå",3*/
//unsigned char car_plate_province_Ji[]	= {0xBC,0xAA};/*"¼ª",4*/
//unsigned char car_plate_province_Liao[] = {0xC1,0xC9};/*"ÁÉ",5*/
//unsigned char car_plate_province_jin[]	= {0xBD,0xFA};/*"½ú",6*/
//unsigned char car_plate_province_ji[]	= {0xBC,0xBD};/*"¼½",7*/
//unsigned char car_plate_province_Qing[] = {0xC7,0xE0};/*"Çà",8*/
//unsigned char car_plate_province_Lu[]	= {0xC2,0xB3};/*"Â³",9*/
//unsigned char car_plate_province_yu[]	= {0xD4,0xA5};/*"Ô¥",10*/
//unsigned char car_plate_province_Su[]	= {0xCB,0xD5};/*"ËÕ",11*/
//unsigned char car_plate_province_Wan[]	= {0xCD,0xEE};/*"Íî",12*/
//unsigned char car_plate_province_Zhe[]	= {0xD5,0xE3};/*"Õã",13*/
//unsigned char car_plate_province_Min[]	= {0xC3,0xF6};/*"Ãö",14*/
//unsigned char car_plate_province_Gan[]	= {0xB8,0xD3};/*"¸Ó",15*/
//unsigned char car_plate_province_Xiang[]= {0xCF,0xE6};/*"Ïæ",16*/
//unsigned char car_plate_province_E[]	= {0xB6,0xF5};/*"¶õ",17*/
//unsigned char car_plate_province_Yue[]	= {0xD4,0xC1};/*"ÔÁ",18*/
//unsigned char car_plate_province_Qiong[]= {0xC7,0xED};/*"Çí",19*/
//unsigned char car_plate_province_gan[]	= {0xB8,0xCA};/*"¸Ê",20*/
//unsigned char car_plate_province_Shan[]	= {0xC9,0xC2};/*"ÉÂ",21*/
//unsigned char car_plate_province_Qian[]	= {0xC7,0xAD};/*"Ç­",22*/
//unsigned char car_plate_province_Dian[]	= {0xB5,0xE1};/*"µá",23*/
//unsigned char car_plate_province_Chuan[]= {0xB4,0xA8};/*"´¨",24*/
//unsigned char car_plate_province_Hei[]	= {0xBA,0xDA};/*"ºÚ",25*/
//unsigned char car_plate_province_Meng[]	= {0xC3,0xC9};/*"ÃÉ",26*/
//unsigned char car_plate_province_Xin[]	= {0xD0,0xC2};/*"ÐÂ",27*/
//unsigned char car_plate_province_Zang[]	= {0xB2,0xD8};/*"²Ø",28*/
//unsigned char car_plate_province_Ning[]	= {0xC4,0xFE};/*"Äþ",29*/
//unsigned char car_plate_province_Gui[]	= {0xB9,0xF0};/*"¹ð",30*/
//unsigned char car_plate_province_Gang[]	= {0xB8,0xDB};/*"¸Û",31*/
//unsigned char car_plate_province_Ao[]	= {0xB0,0xC4};/*"°Ä",32*/

//void printer_info_init(u8 *cmd, u8 *reg_num)
//{
//    printer_info_t *send_cmd = (printer_info_t*)cmd;
//    memcpy(send_cmd->reg_num_chinese, reg_num_china, sizeof(reg_num_china));
//    memset(send_cmd->reg_num, 0x0d, 19);
//    memcpy(send_cmd->reg_num, reg_num, 19);
//}

//void testChinese(u8* ChineseArray)
//{
////    printer_info_t *send_cmd = (printer_info_t*)cmd;
////    memcpy(send_cmd->reg_num_chinese, reg_num_china, sizeof(reg_num_china));
////    memset(send_cmd->reg_num, 0x0d, 19);
////    memcpy(send_cmd->reg_num, reg_num, 19);
//	print_empty_line();
//	testChinese(CarPlateNum);
//	print_empty_line();
//	testChinese(CarPlateClass);
//	print_empty_line();
//	testChinese(DriversLicenseNum);
//	print_empty_line();
//	testChinese(Speed_Status);
//	print_empty_line();
//	testChinese(Print_Time);
//	print_empty_line();
//	testChinese(OverSpeedRecord);
////	printer_send_cmd(set_chinese, sizeof(set_chinese));
//    printer_send_cmd(ChineseArray,sizeof(ChineseArray));
//}

//void printChinese()
//{
////    printer_info_t *send_cmd = (printer_info_t*)cmd;
////    memcpy(send_cmd->reg_num_chinese, reg_num_china, sizeof(reg_num_china));
////    memset(send_cmd->reg_num, 0x0d, 19);
////    memcpy(send_cmd->reg_num, reg_num, 19);
//	print_empty_line();
//	printer_send_cmd(CarPlateNum,sizeof(CarPlateNum));
//	print_empty_line();
//	printer_send_cmd(CarPlateClass,sizeof(CarPlateClass));
//	print_empty_line();
//	printer_send_cmd(DriversLicenseNum,sizeof(DriversLicenseNum));
//	print_empty_line();
//	printer_send_cmd(Speed_Status,sizeof(Speed_Status));
//	print_empty_line();
//	printer_send_cmd(Print_Time,sizeof(Print_Time));
//	print_empty_line();
//	printer_send_cmd(OverSpeedRecord,sizeof(OverSpeedRecord));
//	print_empty_line();
//}

//void printNonChinese(uint8_t* printString)
//{
////	int i;
////	memset(printer_cmd,0,sizeof(printer_cmd));
////	memcpy(printer_cmd, printString, strlen(printString));
//	print_empty_line();
//	printer_send_cmd(printString, strlen(printString));

////	printf("\r\n");
//}


//void printNonChinese(uint8_t* printString,uint16_t printStringLength)
//{
//	memset(printer_cmd,0,sizeof(printer_cmd));
//	memcpy(printer_cmd, printString, sizeof(*printString));
//	print_empty_line();
//	printer_send_cmd(printer_cmd, sizeof(printer_cmd));
//}

//void printNonChineseTest()
//{
//	print_empty_line();
//	sprintf(printer_cmd,"123abcd");
//	printer_send_cmd(printer_cmd,sizeof(printer_cmd));
//}


void Printer_printString(uint8_t* printString)
{
	printer_send_cmd(printString, strlen(printString));
}


void print_empty_line()
{
    printer_send_cmd(out_line, sizeof(out_line));
}
