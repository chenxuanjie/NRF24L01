#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#define TX_ADR_WIDTH   5  // 5???????/????
#define TX_PLOAD_WIDTH 8  // ??????????

/*GPIOÒý½Å¶¨Òå*/
#define NRF24L01_OTHERGPIO		GPIOA
#define NRF24L01_SPI_CE			GPIO_Pin_9
#define NRF24L01_SPI_CSN		GPIO_Pin_8

#define NRF24L01_SPI_GPIO		GPIOB
#define NRF24L01_SPI_CLK		GPIO_Pin_13
#define NRF24L01_SPI_MISO		GPIO_Pin_14
#define NRF24L01_SPI_MOSI		GPIO_Pin_15

#define NRF24L01_IRQ_GPIO		GPIOB
#define NRF24L01_SPI_IRQ		GPIO_Pin_12

// SPI(nRF24L01) commands
#define READ_REGISTER    0x00  // Define read command to register
#define WRITE_REGISTER   0x20  // Define write command to register
#define RD_RX_PLOAD 0x61  // Define RX payload register address
#define WR_TX_PLOAD 0xA0  // Define TX payload register address
#define FLUSH_TX    0xE1  // Define flush TX register command
#define FLUSH_RX    0xE2  // Define flush RX register command
#define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
#define NOP         0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG      0x00  // 'Config' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define CD          0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address

#define NRF24L01_W_CE(x)		GPIO_WriteBit(GPIOA, NRF24L01_SPI_CE, (BitAction)(x))
#define NRF24L01_W_CSN(x)		GPIO_WriteBit(GPIOA, NRF24L01_SPI_CSN, (BitAction)(x))
#define NRF24L01_W_IRQ(x)		GPIO_WriteBit(GPIOB, NRF24L01_SPI_IRQ, (BitAction)(x))

#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
#define ERROR(fmt,arg...)          printf("<<-ERROR->> "fmt"\n",##arg)

void NRF24L01_Init(void);
void RX_Mode(void);
void TX_Mode(uint8_t * BUF);
uint8_t SPI_SendByte(uint8_t Byte);
uint8_t NRF24L01_WriteData(uint8_t Register, uint8_t *Buf, uint8_t Width);
uint8_t NRF24L01_WriteByte(uint8_t Register, uint8_t Data);
uint8_t NRF24L01_ReadByte(uint8_t Register);
uint8_t NRF24L01_ReceiveData(void);
uint8_t NRF24L01_GetData(uint8_t ArrayNum);
void NRF24L01_SetBuf(uint8_t ArrayNum, uint8_t Byte);
void NRF24L01_TransmitBuf(void);
 
#endif
