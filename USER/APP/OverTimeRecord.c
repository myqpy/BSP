#include "OverTimeRecord.h"
#include "terminal_parameter.h"
#include <string.h>
#include "usart.h"


u8 i=0, j=0, pos = 0;
ARM_OvertimeDriveRecord_info OvertimeDriveRecord_info;

u8 handle_overTimeRecord(u8* buf, u8 pos)
{
    
//	OvertimeDrive_ptr = (unsigned char *)malloc(sizeof(unsigned char)*(OvertimeDriveNum*35));
//    pos++;

// 	pos = handle_overTimeRecord(i,pos);
	for(j=0; j<18; j++)
	{
		buf[pos] = OvertimeDriveRecord_info.DriverLicenseNum[j];
		pos++;
	}

	OvertimeDriveRecord_info.startTime.hour = buf[pos];
	pos++;
	OvertimeDriveRecord_info.startTime.min = buf[pos];
	pos++;
	OvertimeDriveRecord_info.startTime.sec = buf[pos];
	pos++;
	OvertimeDriveRecord_info.startTime.w_year = buf[pos] + buf[pos+1];
	pos+=2;
	OvertimeDriveRecord_info.startTime.w_month = buf[pos];
	pos++;
	OvertimeDriveRecord_info.startTime.w_date = buf[pos];
	pos++;

	OvertimeDriveRecord_info.endTime.hour = buf[pos];
	pos++;
	OvertimeDriveRecord_info.endTime.min = buf[pos];
	pos++;
	OvertimeDriveRecord_info.endTime.sec = buf[pos];
	pos++;
	OvertimeDriveRecord_info.endTime.w_year = buf[pos] + buf[pos+1];
	pos+=2;
	OvertimeDriveRecord_info.endTime.w_month = buf[pos];
	pos++;
	OvertimeDriveRecord_info.endTime.w_date = buf[pos];
	pos++;
	
	return pos;
}
