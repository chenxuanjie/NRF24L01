#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "NRF24L01.h"
#include "OLED.h"

uint8_t Status;
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // ??????????
uint8_t RX_BUF[TX_PLOAD_WIDTH];
uint8_t TX_BUF[TX_PLOAD_WIDTH];

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

void NRF24L01_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
																		NRF24L01_SPI_MISO | NRF24L01_SPI_MOSI, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SPI_CLK | NRF24L01_SPI_MISO | NRF24L01_SPI_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//A口推挽输出
	GPIO_InitStructure.GPIO_Pin =NRF24L01_SPI_CSN | NRF24L01_SPI_CE;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//B口的IRQ
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SPI_IRQ;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	NRF24L01_W_CSN(1);
	NRF24L01_W_CE(0);
	NRF24L01_W_IRQ(1);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);	
	
	NRF24L01_WriteByte(WRITE_REGISTER + CONFIG, 0x08);
	NRF24L01_WriteByte(WRITE_REGISTER + EN_AA, 0x3F);
	NRF24L01_WriteByte(WRITE_REGISTER + EN_RXADDR, 0x03);
	NRF24L01_WriteByte(WRITE_REGISTER + SETUP_AW, 0x03);
	NRF24L01_WriteByte(WRITE_REGISTER + SETUP_RETR, 0x03);
	NRF24L01_WriteByte(WRITE_REGISTER + RF_CH, 0x02);
	NRF24L01_WriteByte(WRITE_REGISTER + RF_SETUP, 0x0E);
	NRF24L01_WriteByte(WRITE_REGISTER + STATUS, 0x00);
	NRF24L01_WriteByte(WRITE_REGISTER + RX_PW_P0, 0x00);
	NRF24L01_WriteByte(WRITE_REGISTER + RX_PW_P1, 0x00);
	NRF24L01_WriteByte(WRITE_REGISTER + RX_PW_P2, 0x00);
	NRF24L01_WriteByte(WRITE_REGISTER + RX_PW_P3, 0x00);
	NRF24L01_WriteByte(WRITE_REGISTER + RX_PW_P4, 0x00);
	NRF24L01_WriteByte(WRITE_REGISTER + RX_PW_P5, 0x00);

	// Clear the FIFO's
	NRF24L01_ReadByte(FLUSH_RX);	//??RX_FIFO???
	NRF24L01_ReadByte(FLUSH_TX);	//??RX_FIFO???
}

uint8_t SPI_SendByte(uint8_t Byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	SPI_I2S_SendData(SPI1, Byte);
	while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	}
	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t NRF24L01_WriteByte(uint8_t Register, uint8_t Data)
{
	uint8_t Status;
	NRF24L01_W_CSN(0);
	Status = SPI_SendByte(Register);
	SPI_SendByte(Data);
	NRF24L01_W_CSN(1);
	return Status;	
}

uint8_t NRF24L01_ReadByte(uint8_t Register)
{
	uint8_t Data;
	NRF24L01_W_CSN(0);
	SPI_SendByte(Register);
	Data = SPI_SendByte(0);
	NRF24L01_W_CSN(1);
	return Data;			
}

uint8_t NRF24L01_WriteData(uint8_t Register, uint8_t *Buf, uint8_t Width)
{
	uint8_t Status, i;
	NRF24L01_W_CSN(0);
	Status = SPI_SendByte(Register);
	for (i = 0; i < Width; i ++)
		SPI_SendByte(*(Buf + i));
	NRF24L01_W_CSN(1);
	return Status;
}

uint8_t NRF24L01_ReadData(uint8_t Register, uint8_t *Buf, uint8_t Width)
{
	uint8_t Status, i;
	NRF24L01_W_CSN(0);
	Status = SPI_SendByte(Register);
	for (i = 0; i < Width; i ++)
		*(Buf + i) = SPI_SendByte(0);
	NRF24L01_W_CSN(1);
	return Status;	
}

uint8_t NRF24L01_ReceiveData(void)
{
	uint8_t Value;
	Status = NRF24L01_ReadByte(STATUS);	  // ??????(???4?5?6??????????,??RX_DR)
	if (Status&(0x80>>1))				  // ????????? (??????,?1?????????????1)
	{
		NRF24L01_ReadData(RD_RX_PLOAD, RX_BUF, TX_PLOAD_WIDTH);  // ?RX FIFO???? (?????,1~32??)????????????????,??????
		Value = 1;
	}else{
		Value = 0;
	}
	NRF24L01_WriteByte(WRITE_REGISTER + STATUS, Status);  // ??RX_DS????(???,?????????1,????1????????????)
	NRF24L01_ReadByte(FLUSH_RX);	//??RX_FIFO???
	return Value;
}

uint8_t NRF24L01_GetData(uint8_t ArrayNum)
{
	return RX_BUF[ArrayNum];
}

void RX_Mode(void)
{
	NRF24L01_W_CE(0);
  	NRF24L01_WriteData(WRITE_REGISTER + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // ????????0??????????????
  	NRF24L01_WriteByte(WRITE_REGISTER + EN_AA, 0x01);               // ??????0????
  	NRF24L01_WriteByte(WRITE_REGISTER + EN_RXADDR, 0x01);           // ??????0
  	NRF24L01_WriteByte(WRITE_REGISTER + RF_CH, 40);                 // ??????0x40(????????)
  	NRF24L01_WriteByte(WRITE_REGISTER + RX_PW_P0, TX_PLOAD_WIDTH);  // ????0???????????????
  	NRF24L01_WriteByte(WRITE_REGISTER + RF_SETUP, 0x07);            // ?????1Mbps,????0dBm,????????
  	NRF24L01_WriteByte(WRITE_REGISTER + CONFIG, 0x0f);              // CRC??,16?CRC??,??,????
  	NRF24L01_W_CE(1);                                            // ??CE??????
}

void TX_Mode(uint8_t * BUF)
{
	NRF24L01_W_CE(0);
  	NRF24L01_WriteData(WRITE_REGISTER + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // ??????
  	NRF24L01_WriteData(WRITE_REGISTER + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // ????????,????0?????????
  	NRF24L01_WriteData(WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);                  // ?????TX FIFO
  	NRF24L01_WriteByte(WRITE_REGISTER + EN_AA, 0x01);       // ??????0????
  	NRF24L01_WriteByte(WRITE_REGISTER + EN_RXADDR, 0x01);   // ??????0
  	NRF24L01_WriteByte(WRITE_REGISTER + SETUP_RETR, 0x0a);  // ????????250us+86us,????10?
  	NRF24L01_WriteByte(WRITE_REGISTER + RF_CH, 40);         // ??????0x40(????????)
  	NRF24L01_WriteByte(WRITE_REGISTER + RF_SETUP, 0x07);    // ?????1Mbps,????0dBm,????????
  	NRF24L01_WriteByte(WRITE_REGISTER + CONFIG, 0x0e);      // CRC??,16?cRc??,??
  	NRF24L01_W_CE(1);                                            // ??CE??????
}

uint8_t Check_ACK(uint8_t clear)
{
	while(GPIO_ReadInputDataBit(GPIOB, NRF24L01_SPI_IRQ));								 //??????
	Status = NRF24L01_ReadByte(STATUS);                  
	if(Status&(0x80>>3))
		if(clear)                         // ????TX FIFO,???????MAX_RT???????
			NRF24L01_ReadByte(FLUSH_TX);
	NRF24L01_WriteByte(WRITE_REGISTER + STATUS, Status);  // ??TX_DS?MAX_RT????
	if(Status&(0x80>>2))
		return(0x00);
	else
		return(0xff);
}

void NRF24L01_SetBuf(uint8_t ArrayNum, uint8_t Byte)
{
	TX_BUF[ArrayNum] = Byte;	
}

void NRF24L01_TransmitBuf(void)
{
	TX_Mode(TX_BUF);			
	Check_ACK(1);
}


static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
	OLED_ShowNum(2, 1, errorCode, 1);
	return 0;
}
