#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "NRF24L01.h"
#include "OLED.h"
#include "Delay.h"

uint8_t Flag;
uint8_t Info1, Info2, Info3, Info4, State;

int main(void)
{
	OLED_Init();
	NRF24L01_Init();
	OLED_ShowString(1, 1, "ReceiveNum:");
	OLED_ShowString(2, 1, "2:");
	OLED_ShowString(2, 8, "3:");
	OLED_ShowString(3, 1, "4:");
	RX_Mode();
	while(1)
	{
		State = NRF24L01_ReadByte(STATUS);
		Info1 = NRF24L01_ReadByte(EN_AA);
		Info2 = NRF24L01_ReadByte(EN_RXADDR);
		Info3 = NRF24L01_ReadByte(RX_ADDR_P0);
		Info4 = NRF24L01_ReadByte(FIFO_STATUS);

		OLED_ShowHexNum(4, 1, State, 2);
		OLED_ShowHexNum(4, 4, Info1, 2);
		OLED_ShowHexNum(4, 7, Info2, 2);
		OLED_ShowHexNum(4, 10, Info3, 2);
		OLED_ShowHexNum(4, 13, Info4, 2);
		OLED_ShowNum(2, 16, GPIO_ReadInputDataBit(NRF24L01_IRQ_GPIO, NRF24L01_SPI_IRQ), 1);
		
		Flag = NRF24L01_ReceiveData();
		if (Flag)
		{
			OLED_ShowNum(1, 12, NRF24L01_GetData(NORMAL_TRANSMIT), 4);
			OLED_ShowNum(2, 3, NRF24L01_GetData(FEEDTIME1_TRANSMIT), 4);
			OLED_ShowNum(2, 10, NRF24L01_GetData(FEEDTIME2_TRANSMIT), 4);
			OLED_ShowNum(3, 3, NRF24L01_GetData(FEEDTIME3_TRANSMIT), 4);
		}			
	}
}
