#include "printercmd.h"
#include "printer.h"
#include <string.h>
#include "usart.h"


unsigned char out_line[] = {0x1b,0x66,0x01,0x01};//输出两个空行
u8 printer_cmd[200];
u8 print_temp[200];

//unsigned char set_chinese[] = {0x1c,0x26};
//unsigned char CarPlateNum[] = {0xBB,0xFA, 0xB6,0xAF, 0xB3,0xB5, 0xBA,0xC5, 0xC5,0xC6, 0xBA,0xC5, 0xC2,0xEB, 0xA3,0xBA};//《机动车号牌号码：》    汉字编码
//unsigned char CarPlateClass[] = {0xBB,0xFA, 0xB6,0xAF, 0xB3,0xB5, 0xBA,0xC5, 0xC5,0xC6, 0xB7,0xD6, 0xC0,0xE0, 0xA3,0xBA};//《机动车号牌分类：》    汉字编码
//unsigned char DriversLicenseNum[] = {0xBB,0xFA, 0xB6,0xAF, 0xB3,0xB5, 0xBC,0xDD, 0xCA,0xBB, 0xD6,0xA4, 0xBA,0xC5, 0xC2,0xEB, 0xA3,0xBA};//《机动车驾驶证号码：》    汉字编码
//unsigned char Speed_Status[] = {0xCB,0xD9, 0xB6,0xC8, 0xD7,0xB4, 0xCC,0xAC, 0xA3,0xBA}; //《速度状态：》    汉字编码
//unsigned char Print_Time[] = {0xB4,0xF2,  0xD3,0xA1,  0xCA,0xB1,  0xBC,0xE4, 0xA3,0xBA}; //《打印时间：》    汉字编码
//unsigned char OverSpeedRecord[] = {0xC1,0xBD, 0xB8,0xF6, 0xC8,0xD5, 0xC0,0xFA, 0xCC,0xEC, 0xC4,0xDA, 0xB3,0xAC, 0xCA,0xB1, 0xBC,0xDD, 0xCA,0xBB, 0xBC,0xC7, 0xC2,0xBC, 0xA3,0xBA};//《两个日历天内超时驾驶记录：》    汉字编码

//	
//unsigned char car_plate_province_Jing[]	= {0xBE,0xA9};/*"京",0*/
//unsigned char car_plate_province_Jin[]	= {0xBD,0xF2};/*"津",1*/
//unsigned char car_plate_province_Hu[]	= {0xBB,0xA6};/*"沪",2*/
//unsigned char car_plate_province_Yu[]	= {0xD3,0xE5};/*"渝",3*/
//unsigned char car_plate_province_Ji[]	= {0xBC,0xAA};/*"吉",4*/
//unsigned char car_plate_province_Liao[] = {0xC1,0xC9};/*"辽",5*/
//unsigned char car_plate_province_jin[]	= {0xBD,0xFA};/*"晋",6*/
//unsigned char car_plate_province_ji[]	= {0xBC,0xBD};/*"冀",7*/
//unsigned char car_plate_province_Qing[] = {0xC7,0xE0};/*"青",8*/
//unsigned char car_plate_province_Lu[]	= {0xC2,0xB3};/*"鲁",9*/
//unsigned char car_plate_province_yu[]	= {0xD4,0xA5};/*"豫",10*/
//unsigned char car_plate_province_Su[]	= {0xCB,0xD5};/*"苏",11*/
//unsigned char car_plate_province_Wan[]	= {0xCD,0xEE};/*"皖",12*/
//unsigned char car_plate_province_Zhe[]	= {0xD5,0xE3};/*"浙",13*/
//unsigned char car_plate_province_Min[]	= {0xC3,0xF6};/*"闽",14*/
//unsigned char car_plate_province_Gan[]	= {0xB8,0xD3};/*"赣",15*/
//unsigned char car_plate_province_Xiang[]= {0xCF,0xE6};/*"湘",16*/
//unsigned char car_plate_province_E[]	= {0xB6,0xF5};/*"鄂",17*/
//unsigned char car_plate_province_Yue[]	= {0xD4,0xC1};/*"粤",18*/
//unsigned char car_plate_province_Qiong[]= {0xC7,0xED};/*"琼",19*/
//unsigned char car_plate_province_gan[]	= {0xB8,0xCA};/*"甘",20*/
//unsigned char car_plate_province_Shan[]	= {0xC9,0xC2};/*"陕",21*/
//unsigned char car_plate_province_Qian[]	= {0xC7,0xAD};/*"黔",22*/
//unsigned char car_plate_province_Dian[]	= {0xB5,0xE1};/*"滇",23*/
//unsigned char car_plate_province_Chuan[]= {0xB4,0xA8};/*"川",24*/
//unsigned char car_plate_province_Hei[]	= {0xBA,0xDA};/*"黑",25*/
//unsigned char car_plate_province_Meng[]	= {0xC3,0xC9};/*"蒙",26*/
//unsigned char car_plate_province_Xin[]	= {0xD0,0xC2};/*"新",27*/
//unsigned char car_plate_province_Zang[]	= {0xB2,0xD8};/*"藏",28*/
//unsigned char car_plate_province_Ning[]	= {0xC4,0xFE};/*"宁",29*/
//unsigned char car_plate_province_Gui[]	= {0xB9,0xF0};/*"桂",30*/
//unsigned char car_plate_province_Gang[]	= {0xB8,0xDB};/*"港",31*/
//unsigned char car_plate_province_Ao[]	= {0xB0,0xC4};/*"澳",32*/

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


void print_overTime_record_Header(ARM_vehicle_info rk_vehicle_info)
{
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"机动车号牌号码：", sizeof("机动车号牌号码："));
	Printer_printString(printer_cmd);

	memset(rk_vehicle_info.car_plate_num,0,16);
	memcpy(rk_vehicle_info.car_plate_num,"豫A88888",8);
	Printer_printString(rk_vehicle_info.car_plate_num);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"机动车号牌分类：", sizeof("机动车号牌分类："));
	Printer_printString(printer_cmd);

	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"黄色", sizeof("黄色"));
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"机动车驾驶证号码：", sizeof("机动车驾驶证号码："));
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"410105199607150035", sizeof("410105199607150035"));
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"速度状态：", sizeof("速度状态："));
	Printer_printString(printer_cmd);

	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"正常", sizeof("正常"));
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"打印时间", sizeof("打印时间"));
	Printer_printString(printer_cmd);

	memset(printer_cmd ,0,sizeof(printer_cmd));
	sprintf(printer_cmd,"%04d-%02d-%02d,%02d:%02d:%02d \r\n",calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour,calendar.min,calendar.sec);	
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"两个日历天内超时驾驶记录：", sizeof("两个日历天内超时驾驶记录："));
	Printer_printString(printer_cmd);

	
	print_empty_line();
	print_empty_line();
	print_empty_line();
	print_empty_line();
}

void print_overTime_record_Body(ARM_OvertimeDriveRecord_info OvertimeDriveRecord_info, u8 recordNum)
{
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"记录", sizeof("记录"));
	
	sprintf(print_temp,"%c：",recordNum);
	strncpy(printer_cmd, print_temp, sizeof(print_temp));
	Printer_printString(printer_cmd);
	
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"机动车驾驶证号码：", sizeof("机动车驾驶证号码："));
	Printer_printString(printer_cmd);
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,OvertimeDriveRecord_info.DriverLicenseNum, sizeof(OvertimeDriveRecord_info.DriverLicenseNum));
	Printer_printString(printer_cmd);
	
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"连续驾驶开始时间：", sizeof("连续驾驶开始时间："));
	Printer_printString(printer_cmd);
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	sprintf(printer_cmd,"%04d-%02d-%02d,%02d:%02d:%02d \r\n",OvertimeDriveRecord_info.startTime.w_year, OvertimeDriveRecord_info.startTime.w_month, OvertimeDriveRecord_info.startTime.w_date, OvertimeDriveRecord_info.startTime.hour,OvertimeDriveRecord_info.startTime.min,OvertimeDriveRecord_info.startTime.sec);
	Printer_printString(printer_cmd);
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"连续驾驶结束时间：", sizeof("连续驾驶结束时间："));
	Printer_printString(printer_cmd);	
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	sprintf(printer_cmd,"%04d-%02d-%02d,%02d:%02d:%02d \r\n",OvertimeDriveRecord_info.endTime.w_year, OvertimeDriveRecord_info.endTime.w_month, OvertimeDriveRecord_info.endTime.w_date, OvertimeDriveRecord_info.endTime.hour,OvertimeDriveRecord_info.endTime.min,OvertimeDriveRecord_info.endTime.sec);
	Printer_printString(printer_cmd);	
}
