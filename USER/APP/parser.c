
#include "parser.h"
#include "client_manager.h"
#include "util.h"
#include "usart3.h"

u8 BufferReceive[200];
unsigned int RealBufferReceiveSize=0;
extern MCU_Parameters para;

unsigned short kTerminalParserCMD[8] = {
	kARMGeneralResponse,
	kArmOTrecord,
	kTimeCorrect,
	kSelfCheck,
	kCarInfo,
	kForbidTime,
	kLocation,
	kOTwarning,
};

int parsingMessage(const unsigned char *in, unsigned int in_len)
{
//    unsigned short msg_id;
//    if (FrameParse(in, in_len, &parameter_) < 0)
//    {
//        printf("jt808FrameParse ERROR\r\n");
//        return -1;
//    }
//    msg_id = parameter_.parse.msg_head.msg_id;
	
//	memcpy(BufferReceive, in, in_len);
//	RealBufferReceiveSize = in_len;
	union U16ToU8Array u16converter;
	
	unsigned char pos = 0;
	
	memset(BufferReceive, 0, 200);
	pos++;
	memcpy(BufferReceive, in, in_len);
	para.parse.parser.msg_id = BufferReceive[pos];
	pos++;
	u16converter.u8array[0] = BufferReceive[pos];
	u16converter.u8array[1] = BufferReceive[pos+1];
	para.parse.parser.msg_flow_num = u16converter.u16val;
//	para.packager.msg_flow_num = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
	pos+=2;
	para.parse.parser.msg_length = BufferReceive[pos];
	pos++;

	frameParse(&para);
	
	pos+=para.parse.parser.msg_length;
	
	para.parse.parser.bccCheck = BufferReceive[pos];
	pos++;
	
    return pos;
}

int frameParse(MCU_Parameters *para)
{
	unsigned short msg_id = para->parse.parser.msg_id;
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
	
	// +3399下发时间校准.
	case kTimeCorrect:
	{
		result = handle_kTimeCorrect(para);
	}
	break;
	
	// +3399下发自检结果.
	case kSelfCheck:
	{
		result = handle_kSelfCheck(para);
	}
	break;
	
	// +3399下发车辆信息.
	case kCarInfo:
	{
		result = handle_kCarInfo(para);
	}
	break;
	
	// +3399下发禁行时段.
	case kForbidTime:
	{
		result = handle_kForbidTime(para);
	}
	break;
	
	// +3399下发位置上报.
	case kLocation:
	{
		result = handle_kLocation(para);
	}
	break;
	
	// +3399下发超时预警.
	case kOTwarning:
	{
		result = handle_kOTwarning(para);
	}
	break;
	
	default:
	break;
	}

	return result;
}

int handle_kARMGeneralResponse(MCU_Parameters *para)
{
	union U16ToU8Array u16converter;
	u16converter.u8array[0] = BufferReceive[5];
	u16converter.u8array[1] = BufferReceive[6];
	para->parse.parser.msg_flow_num = u16converter.u16val;
//	para->packager.parse.msg_flow_num_receive = (BufferReceive[5]<<8) + BufferReceive[6];
	para->parse.parser.msg_id = BufferReceive[7];
	
	return 0;
}

int handle_kArmOTrecord(MCU_Parameters *para)
{
	union U16ToU8Array u16converter;
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
	u16converter.u8array[0] = BufferReceive[pos];
	u16converter.u8array[1] = BufferReceive[pos+1];
	para->parse.OvertimeDriveRecord.startTime.w_year = u16converter.u16val;
//	para->parse.OvertimeDriveRecord.startTime.w_year = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
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
	u16converter.u8array[0] = BufferReceive[pos];
	u16converter.u8array[1] = BufferReceive[pos+1];
	para->parse.OvertimeDriveRecord.endTime.w_year = u16converter.u16val;
//	para->parse.OvertimeDriveRecord.endTime.w_year = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
	pos+=2;
	para->parse.OvertimeDriveRecord.endTime.w_month = BufferReceive[pos];
	pos++;
	para->parse.OvertimeDriveRecord.endTime.w_date = BufferReceive[pos];
	pos++;
	
	return 0;
}

int handle_kTimeCorrect(MCU_Parameters *para)
{
	union U16ToU8Array u16converter;
	unsigned char pos = 5;
	
	para->parse.time_info.hour = BufferReceive[pos];
	pos++;
	para->parse.time_info.min = BufferReceive[pos];
	pos++;
	para->parse.time_info.sec = BufferReceive[pos];
	pos++;
	u16converter.u8array[0] = BufferReceive[pos];
	u16converter.u8array[1] = BufferReceive[pos+1];
	para->parse.time_info.w_year = u16converter.u16val;
//	para->parse.time_info.w_year = (BufferReceive[pos]<<8) + BufferReceive[pos+1];
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
	union U16ToU8Array u16converter;
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
	u16converter.u8array[0] = BufferReceive[pos];
	u16converter.u8array[1] = BufferReceive[pos+1];
	para->parse.rk_vehicle_info.pulseRatio = u16converter.u16val;
	
//	para->parse.rk_vehicle_info.pulseRatio = (BufferReceive[pos]<<8)+BufferReceive[pos+1];
	pos+=2;
	return 0;
}

int handle_kForbidTime(MCU_Parameters *para)
{
	para->parse.parser.forbidTime = BufferReceive[5];
	return 0;
}

int handle_kLocation(MCU_Parameters *para)
{
	union U32ToU8Array u32converter;
	union U16ToU8Array u16converter;
	unsigned int i;
	unsigned char pos = 5;
	
	for(i=0;i<4;i++)
	{
		u32converter.u8array[i] = BufferReceive[pos];
		para->parse.Location_info.alarm = u32converter.u32val;
//		para->parse.Location_info.alarm = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
		pos++;
	}
	
	for(i=0;i<4;i++)
	{
		u32converter.u8array[i] = BufferReceive[pos];
		para->parse.Location_info.status = u32converter.u32val;
//		para->parse.Location_info.status = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
		pos++;
	}
	
	for(i=0;i<4;i++)
	{
		u32converter.u8array[i] = BufferReceive[pos];
		para->parse.Location_info.latitude = u32converter.u32val;
//		para->parse.Location_info.latitude = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
		pos++;
	}
	
	for(i=0;i<4;i++)
	{
		u32converter.u8array[i] = BufferReceive[pos];
		para->parse.Location_info.longitude = u32converter.u32val;
//		para->parse.Location_info.longitude = (BufferReceive[pos]<<24)+(BufferReceive[pos+1]<<16)+(BufferReceive[pos+2]<<8)+BufferReceive[pos+3];
		pos++;
	}
	
	u32converter.u8array[0] = BufferReceive[pos];
	u32converter.u8array[1] = BufferReceive[pos+1];
	para->parse.Location_info.altitude = u32converter.u32val;
	pos+=2;
	
	u32converter.u8array[0] = BufferReceive[pos];
	u32converter.u8array[1] = BufferReceive[pos+1];
	para->parse.Location_info.speed = u32converter.u32val;
	pos+=2;
	
	u32converter.u8array[0] = BufferReceive[pos];
	u32converter.u8array[1] = BufferReceive[pos+1];
	para->parse.Location_info.bearing = u32converter.u32val;
	pos+=2;
	
	for(i=0;i<13;i++)	
	{
		para->parse.Location_info.time[i] = BufferReceive[pos];
		pos++;
	}
	return pos;
}

int handle_kOTwarning(MCU_Parameters *para)
{
	para->parse.parser.OTwarning = BufferReceive[5];
	return 0;
}
