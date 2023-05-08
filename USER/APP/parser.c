
#include "parser.h"
#include "client_manager.h"
#include "util.h"
#include "usart3.h"

//MCU_Packager packager;
u8 BufferReceive[200];
unsigned int RealBufferReceiveSize=0;
//unsigned int msg_len = 0;
extern MCU_Parameters para;


int parsingMessage(const unsigned char *in, unsigned int in_len)
{
//    unsigned short msg_id;
//    if (FrameParse(in, in_len, &parameter_) < 0)
//    {
//        printf("jt808FrameParse ERROR\r\n");
//        return -1;
//    }
//    msg_id = parameter_.parse.msg_head.msg_id;
	unsigned char pos = 0;
	pos++;
	memcpy(BufferReceive, in, in_len);
	para.packager.msg_id = BufferReceive[pos];
	pos++;
	para.packager.msg_flow_num = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
	pos+=2;
	para.packager.msg_length = BufferReceive[pos];
	pos++;

	frameParse(&para);
	
	pos+=para.packager.msg_length;
	
	para.packager.parse.bccCheck_receive = BufferReceive[pos];
	
	

    return 0;
}

int frameParse(MCU_Parameters *para)
{
	unsigned short msg_id = para->packager.msg_id;
	int result = -1;
	
	switch (msg_id)
	{
	// +3399通用应答.
	case kARMGeneralResponse:
	{
		result = handle_kARMGeneralResponse(para);
	}
	break;
	
	// +3399下发超时驾驶记录.
	case kArmOTrecord:
	{
		result = handle_kArmOTrecord(para);
	}
	break;
	
	default:
	break;
	}

	return result;
}

int handle_kARMGeneralResponse(MCU_Parameters *para)
{
	para->packager.parse.msg_flow_num_receive = (BufferReceive[5]<<8) + BufferReceive[6];
	para->packager.parse.msg_id_receive = BufferReceive[7];
	
	return 0;
}

int handle_kArmOTrecord(MCU_Parameters *para)
{
	unsigned int i;
	unsigned char pos = 5;
	para->parse.OvertimeDriveRecord.OTnumber = BufferReceive[pos];
	pos++;
	for(i=0;i<18;i++)	
	{
		para->parse.OvertimeDriveRecord.DriverLicenseNum[i] = BufferReceive[pos];
		pos++;
	}
	
	para->parse.OvertimeDriveRecord.startTime.hour = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.startTime.min = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.startTime.sec = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.startTime.w_year = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
	pos+=2;
	para->parse.OvertimeDriveRecord.startTime.w_month = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.startTime.w_date = BufferReceive[pos];
	pos++;

	para->parse.OvertimeDriveRecord.endTime.hour = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.endTime.min = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.endTime.sec = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.endTime.w_year = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
	pos+=2;
	para->parse.OvertimeDriveRecord.endTime.w_month = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.endTime.w_date = BufferReceive[pos];
	pos++;
	
	return 0;
}

int handle_kTimeCorrect(MCU_Parameters *para)
{
	unsigned char pos = 5;
	
	para->parse.time_info.hour = BufferReceive[pos];
	pos++;
	para->parse.time_info.min = BufferReceive[pos];
	pos++;
	para->parse.time_info.sec = BufferReceive[pos];
	pos++;
	para->parse.time_info.w_year = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
	pos+=2;
	para->parse.time_info.w_month = BufferReceive[pos];
	pos++;
	para->parse.time_info.w_date = BufferReceive[pos];
	pos++;
	
	return 0;
}

int handle_kSelfCheck(MCU_Parameters *para)
{
	
	unsigned char pos = 5;
	

	para->parse.selfCheck_info.EC20Status = BufferReceive[pos];
	pos++;
	para->parse.selfCheck_info.EC20SignalStrength = BufferReceive[pos];
	pos++;
	para->parse.selfCheck_info.BDStatus = BufferReceive[pos];
	pos++;
	para->parse.selfCheck_info.SDStatus = BufferReceive[pos];
	pos++;
	para->parse.selfCheck_info.cameraStatus = BufferReceive[pos];
	pos++;
	para->parse.selfCheck_info.velocityStatus = BufferReceive[pos];
	pos++;
	return 0;
}

int handle_kCarInfo(MCU_Parameters *para)
{
	unsigned int i;
	unsigned char pos = 5;
	
	for(i=0;i<12;i++)	
	{
		para->parse.rk_vehicle_info.car_plate_num[i] = BufferReceive[pos];
		pos++;
	}
	para->parse.rk_vehicle_info.car_plate_color = BufferReceive[pos];
	pos++;
	para->parse.rk_vehicle_info.speedLimit = BufferReceive[pos];
	pos++;
	para->parse.rk_vehicle_info.pulseRatio = (BufferReceive[pos]<<8)+BufferReceive[pos+1];
	pos+=2;
	return 0;
}

int handle_kForbidTime(MCU_Parameters *para)
{
	para->packager.parse.forbidTime = BufferReceive[5];
	return 0;
}

int handle_kLocation(MCU_Parameters *para)
{
	unsigned int i;
	unsigned char pos = 5;
	para->parse.Location_info.alarm = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
	pos+=4;
	
	para->parse.Location_info.status = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
	pos+=4;
	
	para->parse.Location_info.latitude = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
	pos+=4;
	
	para->parse.Location_info.longitude = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
	pos+=4;
	
	para->parse.Location_info.altitude = (BufferReceive[pos]<<8)+BufferReceive[pos+1];
	pos+=2;
	
	para->parse.Location_info.speed = (BufferReceive[pos]<<8)+BufferReceive[pos+1];
	pos+=2;
	
	para->parse.Location_info.bearing = (BufferReceive[pos]<<8)+BufferReceive[pos+1];
	pos+=2;
	
	for(i=0;i<13;i++)	
	{
		para->parse.Location_info.time[i] = BufferReceive[pos];
		pos++;
	}
	return 0;
}

int handle_kOTwarning(MCU_Parameters *para)
{
	para->packager.parse.OTwarning = BufferReceive[5];
	return 0;
}
