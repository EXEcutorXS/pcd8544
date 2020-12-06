#include "PCD8544.h"
#include "main.h"

SPI_HandleTypeDef* hspi;
extern const unsigned char myFont[165][5];
/**
* @brief Writes Data/Command into LCD
* @param  dc: data(1)/command(0), data: data byte
* @retval None
*/
void PCD_Write (unsigned char dc, uint8_t data)
{
	HAL_GPIO_WritePin(DC_PORT, DC_Pin, dc);
	HAL_SPI_Transmit(hspi, &data, 1, 100);
}

/**
* @brief Clears LCD by multiple writing 0
* @param  None
* @retval None
*/
void PCD_Clear(void)
{
	for (int index = 0; index < PCD_WIDTH * PCD_HEIGHT / 8; index++)
		PCD_Write(DATA, 0x00);

}
/**
* @brief Initialises LCD on SPI
* @param  spi: SPI handler, which controls LCD
* @retval None
*/
void PCD_Init(SPI_HandleTypeDef* spi)
{
	hspi=spi;
	HAL_GPIO_WritePin(RST_PORT,RST_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RST_PORT,RST_Pin,GPIO_PIN_SET);
	PCD_Write(COMMAND, 0x21 );  // LCD Extended Commands.
	PCD_Write(COMMAND, 0xBE);  // Set LCD Vop (Contrast).
	PCD_Write(COMMAND, 0x014 );  // Set Temp coeffiCEEnt. //0x04
	PCD_Write(COMMAND, 0x14 );  // LCD bias mode 1:48. //0x13
	PCD_Write(COMMAND, 0x20 );  // LCD Basic Commands
	PCD_Write(COMMAND, 0x0C );  // LCD in normal mode.0x0D
}

/**
* @brief Sets LCD cursor position
* @param  x:column(0..83),y:row(0..5)
* @retval None
*/
void PCD_GotoXY(int x, int y)
{
	PCD_Write(COMMAND, 0x80 | x);  // Column.
	PCD_Write(COMMAND, 0x40 | y);  // Row.

}

/**
* @brief Puts Cyrillyc symbol in UTF-16 on LCD
* @param  c: pointer to 16bit symbol
* @retval None
*/
void PCD_PutRuc(char* c)
{
	uint8_t i;
	uint8_t pos;
	if (*c==0xD0) pos=*(c+1)-144+101;
	else if (*c==0xD1) pos=*(c+1)-128+149;
	else return;
	for(i=0;i<5;i++)
		PCD_Write(1, myFont[pos][i]);
}

/**
* @brief Puts symbol on LCD
* @param  c: char
* @retval None
*/
void PCD_Putc(char c)
{
	uint8_t i;
	for(i=0;i<5;i++)
		PCD_Write(DATA, myFont[c-0x20][i]);
}

/**
* @brief Pust string on LCD
* @param  c: string pointer
* @retval None
*/
void PCD_Puts(char *c)
{
	while (*c) {
		if (*c==0xD0 || *c==0xD1)
		{
			PCD_PutRuc(c);
			c+=2;
		}
		else
			PCD_Putc(*c++);
		PCD_Write(1, 0);//Space between characters
	}
}

/**
* @brief Draws full-size(84*48) bitmap on LCD
* @param  ptr: pointer to bitmap array
* @retval None
*/
void PCD_DrawBitmap(uint8_t * ptr)
{
	uint16_t i=0;
	PCD_GotoXY(0, 0);
	for (i=0;i<PCD_BUFFER_SIZE;i++)
		PCD_Write(DATA, *ptr++);

}
