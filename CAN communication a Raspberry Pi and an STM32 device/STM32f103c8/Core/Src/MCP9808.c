#include "MCP9808.h"

uint8_t readBuffer[5];
uint8_t writeBuffer;

static void _WriteAddr(MCP9808_TypeDef* tempSensor, uint8_t REG, uint8_t data) {
	uint8_t payload[3];
	payload[0] = REG;
	payload[1] = data;
	HAL_I2C_Master_Transmit(&tempSensor->hi2c, tempSensor->devAddr<<1, payload, 2, HAL_MAX_DELAY);
}

static void _ReadAddr(MCP9808_TypeDef *tempSensor, uint8_t REG, unsigned char buffer[5]) {
	uint8_t readReg = REG;// << 3);
	HAL_I2C_Master_Transmit(&tempSensor->hi2c, tempSensor->devAddr<<1, &readReg, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&tempSensor->hi2c, tempSensor->devAddr<<1, buffer, 2, HAL_MAX_DELAY);
}

void MCP9808_Init(MCP9808_TypeDef *tempSensor, uint8_t devAddr, I2C_HandleTypeDef hi2c) {
	tempSensor->devAddr = (MCP9808_ADDR | devAddr);
	tempSensor->hi2c = hi2c;
	_WriteAddr(tempSensor, MCP9808_REG_CONF, (uint8_t) 0x00);
	//_ReadAddr(&tempSensor, 0x08, readBuffer);
	//HAL_Delay(100);
}

uint8_t MCP9808_GetResolution(MCP9808_TypeDef *tempSensor) {
	_ReadAddr(tempSensor, MCP9808_REG_RESOLUTION, readBuffer);
	//_ReadAddr(tempSensor, MCP9808_REG_TEMP, readBuffer);
	//HAL_Delay(100);
	return readBuffer[0];
}

uint8_t MCP9808_SetResolution(MCP9808_TypeDef *tempSensor, uint8_t value) {
	_WriteAddr(tempSensor, MCP9808_REG_RESOLUTION, value & 0x03);
	//HAL_Delay(100);
	return readBuffer[0];
}

static uint16_t _ReadTemperature(MCP9808_TypeDef *tempSensor) {
	_ReadAddr(tempSensor, MCP9808_REG_TEMP, readBuffer);
	uint16_t t = word(readBuffer[0], readBuffer[1]);
	return t;
}

uint16_t* MCP9808_ReadTempC(MCP9808_TypeDef *tempSensor) {
	static uint16_t temp[2] = {0};
	uint16_t t = _ReadTemperature(tempSensor);
	if (t != 0xFFFF) {
		temp[0] = t & 0x0FFF;
		temp[1] = temp[0] % 16;
		temp[0] /= 16.0;
		if (t & 0x1000) temp[0] -= 256;
	}
	return temp;

}

static void _ShutdownWake(MCP9808_TypeDef *tempSensor, uint8_t sw) {
	_ReadAddr(tempSensor, MCP9808_REG_CONF, readBuffer);
	uint16_t confREG = word(readBuffer[0], readBuffer[1]);
	uint16_t confShutdown;
	if (sw == 1) {
		confShutdown = confREG | MCP9808_REG_CONF_SHUTDOWN;
	} else {
		confShutdown = confREG & ~MCP9808_REG_CONF_SHUTDOWN;
	}
	_WriteAddr(tempSensor, MCP9808_REG_CONF, confShutdown);
}

void MCP9808_Shutdown(MCP9808_TypeDef* tempSensor) {
	_ShutdownWake(tempSensor, 1);
}

void MCP9808_Wake(MCP9808_TypeDef* tempSensor) {
	_ShutdownWake(tempSensor, 0);
}
