//-----------------------------------------------------------------------------
// Библиотека для работы с PCD8544
// Разработана для CubeIDE
// Кодировка - UTF-8
// Поддерживается кириллица
// Для работы необходимо настроить порты RESET и DC
// Сигнал CE - аппаратный
//-------
#include "stm32f1xx_hal.h"


#ifndef __PCD_H_
    #define __PCD_H_

// Начало настроек
#define DC_PORT  GPIOB
#define RST_PORT GPIOA
#define DC_Pin   GPIO_PIN_14
#define RST_Pin  GPIO_PIN_8
//Конец настроек

#define COMMAND   0
#define DATA      1

#define PCD_WIDTH				84
#define PCD_HEIGHT				48

#define PCD_BUFFER_SIZE 		PCD_WIDTH * PCD_HEIGHT / 8

void PCD_Write (unsigned char dc, uint8_t data);
void PCD_Clear(void);
void PCD_Init(SPI_HandleTypeDef* spi);
void PCD_GotoXY(int x, int y);
void PCD_PutRuc(char* c);
void PCD_Putc(char c);
void PCD_Puts(char *c);
void PCD_DrawBitmap(uint8_t * ptr);
#endif
