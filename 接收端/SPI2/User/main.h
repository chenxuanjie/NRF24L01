#ifndef __MAIN_H__
#define __MAIN_H__

//发送的第n个数组
#define NORMAL_TRANSMIT			0
#define KEY_TRANSMIT			1
#define SWITCH_TRANSMIT			2
#define ROCKER_TRANSMIT			3
#define ENCODER_TRANSMIT		4
#define FEEDTIME1_TRANSMIT		5
#define FEEDTIME2_TRANSMIT		6
#define FEEDTIME3_TRANSMIT		7

//按键 KEY_TRANSMIT
#define KEY_ROCKER_TRANSMIT			0x1

//摇杆 ROCKER_TRANSMIT
#define UP_1					0x1
#define UP_2					0x2
#define UP_3					0x3
#define UP_4					0x4
#define DOWN_1					0x5
#define DOWN_2					0x6
#define DOWN_3					0x7
#define DOWN_4					0x8
#define LEFT_1					0x1
#define LEFT_2					0x2
#define LEFT_3					0x3
#define LEFT_4					0x4
#define RIGHT_1					0x5
#define RIGHT_2					0x6
#define RIGHT_3					0x7
#define RIGHT_4					0x8
#define MEDIWM					0x99


#endif
