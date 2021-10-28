#pragma once

#include <AP_HAL/I2CDevice.h>

//#define I2C_BUS 1
//#define ADDRESS 0x28

class SPL_MotorController{
public:
	uint16_t values[3];
	void sendValues();
	void init(uint8_t address);
private:
	AP_HAL::OwnPtr<AP_HAL::I2CDevice> _motorController = nullptr;
	uint32_t buildPacket();
};