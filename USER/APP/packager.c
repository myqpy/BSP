#include "delay.h"
#include "packager.h"
#include "client_manager.h"
#include "util.h"
#include "usart3.h"

unsigned short packagerCMD[4] = {
	kMCUGeneralResponse,
	kMCUStatusReport,
	kMCUAlarmReport,
	kAcquireOTReport,
};
uint8_t McuPackage[BUFFER_SIZE_SEND];
unsigned int RealBufferSendSize=0;
unsigned int msg_len = 0;

void bufferSendPushByte(unsigned char byte)
{
    McuPackage[RealBufferSendSize] = byte;
    ++RealBufferSendSize;
}

int bufferSendPushBytes(const unsigned char *bytes, unsigned int size)
{
    unsigned int i;
    if (bytes == NULL)
    {
        return 0;
    }
    for (i = 0; i < size; ++i)
    {
        bufferSendPushByte(*(bytes + i));
    }

    return 1;
}

int McuMsgPackage(MCU_Parameters *para)
{
	union U16ToU8Array u16converter;
//    unsigned char ret;
    unsigned char valueCheck;
    // 清空发送缓存，避免缓存中会有前一次的数据
	memset(McuPackage, 0, 200);
	RealBufferSendSize = 0;
	
    //设置头标志位
	bufferSendPushByte(0xEE);
	
	//设置消息ID
	bufferSendPushByte(para->packager.msg_id);

	//设置流水号
//	u16converter.u16val = EndianSwap16(para->packager.msg_flow_num);
	u16converter.u16val = para->packager.msg_flow_num;
	bufferSendPushBytes(u16converter.u8array, 2);	

	RealBufferSendSize++;
		
    //封装消息内容.
    para->packager.msg_length = McuFrameBodyPackage(para);
	//设置消息长度
	McuPackage[4] = para->packager.msg_length;


	// 4、获取校验码，并将其写入发送缓存.
	valueCheck = BccCheckSum((McuPackage+1), (RealBufferSendSize-1));//参考接收解析
	bufferSendPushByte(valueCheck);

	// 5、写入发送缓存结束标识位.
	bufferSendPushByte(0xEE);

	

	
	return 0;

}

int McuFrameBodyPackage(MCU_Parameters *para)
{
    unsigned short msg_id = para->packager.msg_id;
    int result = -1;
#ifdef __JT808_DEBUG
    printf("[jt808FrameBodyPackage] msg_id: 0x%04x\r\n", para->msg_head.msg_id);
#endif

    switch (msg_id)
    {
    // 32通用应答.
    case kMCUGeneralResponse:
    {
        result = handle_GeneralResponse(para);
    }
    break;

    // 32上报状态信息.
    case kMCUStatusReport:
    {
        result = handle_StatusReport(para);
    }
    break;

    // 32上报报警信息.
    case kMCUAlarmReport:
    {
        result = handle_AlarmReport(para);
    }
    break;

    // 32请求超时驾驶记录.
    case kAcquireOTReport:
    {
        result = handle_AcquireOTReport(para);
    }
    break;

    default:
        break;
    }

    return result;
}

int handle_GeneralResponse(MCU_Parameters *para)
{
	union U16ToU8Array u16converter;
	
	msg_len=0;
	
	//应答流水号
//	u16converter.u16val = EndianSwap16(para->packager.parse.msg_flow_num_receive);
	u16converter.u16val = para->parse.parser.msg_flow_num;
	bufferSendPushBytes(u16converter.u8array, 2);
	msg_len+=2;
	
	//应答ID
	bufferSendPushByte(para->parse.parser.msg_id);
	msg_len++;
	
	//应答结果
	bufferSendPushByte(0x0);
	msg_len++;
	
	return msg_len;
}
int handle_StatusReport(MCU_Parameters *para)
{
	union U32ToU8Array u32converter;
	unsigned int i;
	msg_len=0;
	
//	u32converter.u32val = EndianSwap32(para->mcu_car_info.mileage);
	u32converter.u32val = para->mcu_car_info.mileage;
	bufferSendPushBytes(u32converter.u8array,4);
	msg_len+=4;
	
//	u32converter.u32val = EndianSwap32(para->mcu_car_info.velocity);
	u32converter.u32val = para->mcu_car_info.velocity;
	bufferSendPushBytes(u32converter.u8array,4);
	msg_len+=4;
	
	for(i=0;i<18;i++) bufferSendPushByte(para->mcu_car_info.driver_num[i]);
	msg_len+=18;
	
	bufferSendPushByte(para->mcu_car_info.fire);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.brake);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.left);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.right);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.low_beam);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.hign_beam);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.isCharged);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.LoadingStatus);
	msg_len++;
	bufferSendPushByte(para->mcu_car_info.ICcardStatus);
	msg_len++;
	
	
	return msg_len;
}
int handle_AlarmReport(MCU_Parameters *para)
{
	msg_len=0;
	bufferSendPushByte(para->packager.statusBit);
	msg_len++;
	bufferSendPushByte(para->packager.statusValue);
	msg_len++;
	return msg_len;
}
int handle_AcquireOTReport(MCU_Parameters *para)
{
	msg_len=0;
	bufferSendPushByte(para->packager.OTpageNum);
	return msg_len;
}

int findMsgIDFromTerminalPackagerCMD(unsigned int msg_id)
{
    int result = 0;
    int i;
    for (i = 0; i < 4; ++i)
    {
        if (packagerCMD[i] == msg_id)
        {
            result = 1;
        }
    }
    return result;
}

