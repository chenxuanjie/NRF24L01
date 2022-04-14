#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "NRF24L01.h"
#include "OLED.h"

uint16_t i;
uint8_t Info1, Info2, Info3, Info4, State;

int main(void)
{
	OLED_Init();
	NRF24L01_Init();
	OLED_ShowString(1, 1, "SendNum:");
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
		
		NRF24L01_SetBuf(NORMAL_TRANSMIT, i ++);
		NRF24L01_TransmitBuf();		
		OLED_ShowNum(1, 9, i, 4);
	}
}
