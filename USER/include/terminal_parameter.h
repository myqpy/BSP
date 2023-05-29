#ifndef TERMINAL_PARAMETER_H_
#define TERMINAL_PARAMETER_H_

#include "sys.h"
#include "rtc.h"
//#pragma pack(push)
//#pragma pack(1) // 结构体1字节对齐
//typedef struct TerminalParameters_str
//{
//    uint32_t impulse_ratio;//脉冲系数，车辆每前进1公里收到的脉冲数
//    uint32_t mileage;
//} TerminalParameters_t;
//#pragma pack() // 恢复默认字节对齐

enum packagerCMD
{
	kMCUGeneralResponse = 0x01,
	kMCUStatusReport,
	kMCUAlarmReport,
	kAcquireOTReport,
	kMCUCANReport,
};

enum parserCMD
{
	kARMGeneralResponse = 0x51,
	kArmOTrecord,
	kTimeCorrect,
	kSelfCheck,
	kCarInfo,
	kForbidTime,
	kLocation,
	kOTwarning,
	kZeroMileage,
	kcheckCommand,
};

enum EN_StatusBit
{
//    kLoadingStatus 	= 	0x0,	// 载货状态	1 满载 0 空载
    kSOS			= 	0x1,	// 人工报警	1 报警
    kOverSpeed	 	=	0x2,	// 超速提醒 	1 报警
    kLowVoltage		= 	0x3,	// 电瓶欠压	1 报警
    kOverTime		=	0x4,	// 超时驾驶	1 报警
//    kICcard			=	0x5,	// 人员登录 	1 登录 0 登出
//	kACCoff			=	0x6,	// 断电提醒	1 报警
};

//#pragma pack(push)
//#pragma pack(1) // 结构体1字节对齐
//typedef struct Struct_ARM_time_info
//{
////    uint8_t header;
//    uint8_t hour;
//    uint8_t min;
//    uint8_t sec;
//    //公历日月年周
//    uint16_t int w_year;
//    uint8_t  w_month;
//    uint8_t  w_date;
//} ARM_time_info;
//#pragma pack() // 恢复默认字节对齐


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_ARM_selfCheck_info
{
    uint8_t 	EC20Status;
    uint8_t		EC20SignalStrength;
    uint8_t 	BDStatus;
    uint8_t 	SDStatus;
    uint8_t 	cameraStatus;
    uint8_t 	velocityStatus;
} ARM_selfCheck_info;
#pragma pack() // 恢复默认字节对齐


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_ARM_vehicle_info
{
//	uint8_t 	header;
    uint8_t		car_plate_num[12];	//机动车号牌号码
    uint8_t		car_plate_color;	//机动车号牌颜色
    uint8_t		speedLimit;			//速度阈值
    uint16_t	pulseRatio;			//脉冲系数
} ARM_vehicle_info;
#pragma pack() // 恢复默认字节对齐

//#pragma pack(push)
//#pragma pack(1) // 结构体1字节对齐
//typedef struct Struct_ARM_OvertimeDriveTime
//{
//	vuint8_t hour;
//	vuint8_t min;
//	vuint8_t sec;
//	//公历日月年周
//	vuint16_t w_year;
//	vuint8_t  w_month;
//	vuint8_t  w_date;
//}ARM_OvertimeDriveTime;
//#pragma pack() // 恢复默认字节对齐


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_TimeInfo
{
	vu8 hour;
	vu8 min;
	vu8 sec;
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
} TimeInfo;
#pragma pack() // 恢复默认字节对齐

#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct BCD_TimeInfo
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t h;
	uint8_t m;
	uint8_t	s;
} bcd_timeinfo;
#pragma pack() // 恢复默认字节对齐


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_ARM_OvertimeDriveRecord
{
    uint8_t	OTnumber;
    uint8_t	DriverLicenseNum[18];
    bcd_timeinfo	startTime;
    bcd_timeinfo	endTime;
} ARM_OvertimeDriveRecord;
#pragma pack() // 恢复默认字节对齐

#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_ARM_Location_info
{
    // 报警标志 4B
    int alarm;
    // 状态位定义 4B
    int status;
    // 纬度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
    uint32_t latitude;
    // 经度(以度为单位的纬度值乘以10的6次方, 精确到百万分之一度) 4B
    uint32_t longitude;
    // 海拔高度, 单位为米(m) 2B
    uint16_t altitude;
    // 速度 1/10km/h 2B
    uint16_t speed;
    // 方向 0-359,正北为0, 顺时针 2B
    uint16_t bearing;
    // 时间, "YYMMDDhhmmss"(GMT+8时间, 本标准之后涉及的时间均采用此时区).12B
    // std::string time;
    uint8_t time[13];
} ARM_Location_info;


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_MCU_Packager
{
    uint8_t		msg_id;			//打包消息id
    uint16_t 	msg_flow_num;	//打包流水号
    uint8_t		msg_length;		//打包消息长度
    uint8_t		statusBit;		//车辆状态位
    uint8_t 	statusValue;	//车辆状态值
    uint8_t		OTpageNum;		//超时驾驶记录请求
	uint16_t	CANnum;			//CAN数据项个数
} MCU_Packager;
#pragma pack() // 恢复默认字节对齐


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_MCU_Parser
{
	uint8_t		msg_id;				//解析消息id
	uint16_t	msg_flow_num;		//解析流水号
	uint8_t		response_id;		//解析通用应答消息id
	uint16_t	response_flow_num;	//解析通用应答流水号
	uint8_t		response_result;	//解析通用应答结果
	uint8_t		msg_length;			//解析消息长度
	uint8_t		bccCheck;			//异或校验
	uint8_t		forbidTime;			//禁行时段
	uint8_t 	OTwarning;			//超时预警
	uint8_t		zeroMileage;		//里程清零
	uint8_t		checkCommand;		//检定命令字
} MCU_Parser;
#pragma pack() // 恢复默认字节对齐

#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_MCU_car_info
{
    uint32_t mileage;
    uint32_t velocity;
    uint8_t driver_num[18];//机动车驾驶证号码
    uint8_t fire;//点火状态
    uint8_t brake;//刹车状态
    uint8_t left;//左转灯
    uint8_t right;//右转灯
    uint8_t low_beam;//近光灯
    uint8_t hign_beam;//远光灯
    uint8_t isCharged; //外部供电or电池供电
	uint8_t LoadingStatus;// 载货状态	3 满载 0 空载
	uint8_t ICcardStatus;		// 人员登录 	1 登录 0 登出
	uint32_t drive_time;

} MCU_car_info;
#pragma pack() // 恢复默认字节对齐



#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_MCU_ICcard_info
{
    uint32_t	Reserved;
    uint8_t	DriverLicenseNum[18];
    uint8_t	DriverLicenseValid_Year;
    uint8_t	DriverLicenseValid_Month;
    uint8_t	DriverLicenseValid_Date;
    uint8_t	EmployQualifCertifiNum[18];
    uint8_t	StandardExtensionReservation[56];
    uint8_t	BCCchecksum;
} MCU_ICcard_info;
#pragma pack() // 恢复默认字节对齐

#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_MCU_OT_info
{
    uint8_t	OTpageNum_Show;			//超时驾驶记录显示
	uint8_t	OTpageNum_print;//超时驾驶记录打印
	uint8_t	print_flag;
} MCU_OT_info;
#pragma pack() // 恢复默认字节对齐

// CAN_ID
union UN_CAN_ID
{
  struct
  {
	  //CAN总线ID
	  unsigned int 	CAN_stdID:11;		//CAN总线标准帧ID
	  unsigned int 	CAN_extID:11;		//CAN总线扩展帧ID
	  unsigned char	CAN_dataCollect:1;	//数据采集格式
	  unsigned char	CAN_IDE:1;			//帧格式
	  unsigned char CAN_Channel:1;		//通道号
  } bit;
  unsigned int value;
};


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_MCU_CAN
{
	union UN_CAN_ID CAN_id;
	uint8_t		CAN_data[8];
} MCU_CAN;

#pragma pack() // 恢复默认字节对齐


#pragma pack(push)
#pragma pack(1) // 结构体1字节对齐	
typedef struct Struct_MCU_Parameters
{
    MCU_Packager	packager;
    MCU_ICcard_info ICcard_info;
    MCU_car_info	mcu_car_info;
	MCU_OT_info		OT_info;
	MCU_CAN			CAN_info;
    struct
    {
		TimeInfo 				time_info;
		ARM_selfCheck_info		selfCheck_info;
		ARM_vehicle_info		rk_vehicle_info;
		ARM_OvertimeDriveRecord	OvertimeDriveRecord;
		ARM_Location_info		Location_info;
		MCU_Parser				parser;
		bcd_timeinfo			bcdtime;
    } parse;

} MCU_Parameters;

#pragma pack() // 恢复默认字节对齐



#endif // TERMINAL_PARAMETER_H_
