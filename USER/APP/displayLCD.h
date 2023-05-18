
#include "sys.h"	    
#include "terminal_parameter.h"




extern uint8_t vehicle_driver_info[][24];
extern uint8_t overTimeDriveRecord[][24];
extern uint8_t car_plate_color[][24];
extern uint8_t Chinese_car_plate[][24];
extern uint8_t pulseRatio[][24];
extern uint8_t car_plate_province[][24];
extern uint8_t car_IO_status[][24];
extern uint8_t changeLoadingStatus[][24];
extern uint8_t forbidTimeArray[][24];
extern uint8_t nike[][16];
extern uint8_t icon_empty[][16];
extern uint8_t OTBeginEndTime[][24];
extern uint8_t more[][24];


void LCD_Clear(void);
void showMainMenu(MCU_Parameters *para);
void displayEmpty(uint8_t page,uint8_t CaddrH,uint8_t CaddrL);
void displayChinese_16x16(uint8_t page,uint8_t CaddrH,uint8_t CaddrL,uint8_t arr[][24],uint8_t startWord, uint8_t endWord);

void displayWeekday(uint8_t page,uint8_t CaddrH,uint8_t CaddrL,int weekday);


void displayIcon(uint8_t page,uint8_t CaddrH,uint8_t CaddrL, uint8_t arr[2][16]);
int displayCarPlateHead(uint8_t* car_plate_num);


int WeekYearday(int years, int months, int days);


















