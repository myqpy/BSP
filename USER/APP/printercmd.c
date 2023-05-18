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


void print_overTime_record_Header(MCU_Parameters *para)
{
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"机动车号牌号码：", sizeof("机动车号牌号码："));
	Printer_printString(printer_cmd);

//	memset(rk_vehicle_info->car_plate_num,0,12);
//	memcpy(rk_vehicle_info->car_plate_num,"豫A88888",8);
	Printer_printString(para->parse.rk_vehicle_info.car_plate_num);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"机动车号牌分类：", sizeof("机动车号牌分类："));
	Printer_printString(printer_cmd);

	memset(printer_cmd ,0,sizeof(printer_cmd));
	if(para->parse.rk_vehicle_info.car_plate_color==0x00) memcpy(printer_cmd,"未上牌", sizeof("未上牌"));
	else if(para->parse.rk_vehicle_info.car_plate_color==0x01) memcpy(printer_cmd,"蓝色", sizeof("蓝色"));
	else if(para->parse.rk_vehicle_info.car_plate_color==0x02) memcpy(printer_cmd,"黄色", sizeof("黄色"));
	else if(para->parse.rk_vehicle_info.car_plate_color==0x03) memcpy(printer_cmd,"黑色", sizeof("黑色"));
	else if(para->parse.rk_vehicle_info.car_plate_color==0x04) memcpy(printer_cmd,"白色", sizeof("白色"));
	else memcpy(printer_cmd,"其他", sizeof("其他"));
	
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"机动车驾驶证号码：", sizeof("机动车驾驶证号码："));
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,&para->mcu_car_info.driver_num, sizeof(para->mcu_car_info.driver_num));
	Printer_printString(printer_cmd);

	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"速度状态：", sizeof("速度状态："));
	Printer_printString(printer_cmd);

	memset(printer_cmd ,0,sizeof(printer_cmd));
	if(para->parse.selfCheck_info.velocityStatus==0x00) memcpy(printer_cmd,"正常", sizeof("正常"));
	if(para->parse.selfCheck_info.velocityStatus==0x01) memcpy(printer_cmd,"异常", sizeof("异常"));
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

void print_overTime_record_Body(MCU_Parameters *para, u8 recordNum)
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
	memcpy(printer_cmd,&para->parse.OvertimeDriveRecord.DriverLicenseNum, sizeof(para->parse.OvertimeDriveRecord.DriverLicenseNum));
	Printer_printString(printer_cmd);
	
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"连续驾驶开始时间：", sizeof("连续驾驶开始时间："));
	Printer_printString(printer_cmd);
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	sprintf(printer_cmd,"%04d-%02d-%02d,%02d:%02d:%02d \r\n",para->parse.OvertimeDriveRecord.startTime.year, para->parse.OvertimeDriveRecord.startTime.month, para->parse.OvertimeDriveRecord.startTime.date, para->parse.OvertimeDriveRecord.startTime.h,para->parse.OvertimeDriveRecord.startTime.m,para->parse.OvertimeDriveRecord.startTime.s);
	Printer_printString(printer_cmd);
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	memcpy(printer_cmd,"连续驾驶结束时间：", sizeof("连续驾驶结束时间："));
	Printer_printString(printer_cmd);	
	
	print_empty_line();
	memset(printer_cmd ,0,sizeof(printer_cmd));
	sprintf(printer_cmd,"%04d-%02d-%02d,%02d:%02d:%02d \r\n",para->parse.OvertimeDriveRecord.endTime.year, para->parse.OvertimeDriveRecord.endTime.month, para->parse.OvertimeDriveRecord.endTime.date, para->parse.OvertimeDriveRecord.endTime.h,para->parse.OvertimeDriveRecord.endTime.m,para->parse.OvertimeDriveRecord.endTime.s);
	Printer_printString(printer_cmd);	
}
