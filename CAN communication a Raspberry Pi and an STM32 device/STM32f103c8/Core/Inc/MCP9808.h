 /*
 * MCP9808.h
 *
 *  Created on: 30.09.2020
 *      Author: SimpleMethod
 *
 *Copyright 2020 SimpleMethod
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of
 *this software and associated documentation files (the "Software"), to deal in
 *the Software without restriction, including without limitation the rights to
 *use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *of the Software, and to permit persons to whom the Software is furnished to do
 *so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 ******************************************************************************
 */


#ifndef INC_MCP9808_H_
#define INC_MCP9808_H_
#endif

#include "main.h"
#include <stdbool.h>
#include "stm32f1xx.h"

/*!
 * ADDRESS REGISTER
 */

#define MCP9808_REG_TEMP	 			0x05
#define MCP9808_ADDR					0x18
#define MCP9808_REG_CONF				0x01
#define MCP9808_REG_CONF_SHUTDOWN		0x0100
#define MCP9808_REG_RESOLUTION		 	0x08

#define word(x,y) (((x) << 8) | (y))

typedef struct {
	I2C_HandleTypeDef hi2c;
	uint8_t devAddr;
} MCP9808_TypeDef;

void MCP9808_Init(MCP9808_TypeDef *tempSensor, uint8_t devAddr, I2C_HandleTypeDef hi2c);
uint8_t MCP9808_GetResolution(MCP9808_TypeDef *tempSensor);
uint8_t MCP9808_SetResolution(MCP9808_TypeDef *tempSensor, uint8_t value);
uint16_t* MCP9808_ReadTempC(MCP9808_TypeDef *tempSensor);
void MCP9808_Shutdown(MCP9808_TypeDef* tempSensor);
void MCP9808_Wake(MCP9808_TypeDef* tempSensor);
