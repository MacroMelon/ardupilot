#include <GCS_MAVLink/GCS.h>
#include "AC_SPL_Motor_Ctrl.h"
#include <AP_HAL/AP_HAL.h>

extern const AP_HAL::HAL& hal;

void SPL_MotorController::init(uint8_t address) {
	_motorController = hal.i2c_mgr->get_device(1, address);
	if (!_motorController) {
		hal.console->printf("No controller?");
		gcs().send_text(MAV_SEVERITY_INFO, "AP_WNDVN_AS5048B Constructed.");
		return;
	}
}


void SPL_MotorController::sendValues(){
	uint32_t packet_data = buildPacket();
	if(!_motorController->get_semaphore()->take(2)){
		return;	//todo- better handling error
	}
	_motorController->transfer(reinterpret_cast<const uint8_t *>(&packet_data), 4, NULL, 0);
	_motorController->get_semaphore()->give();
}

uint32_t SPL_MotorController::buildPacket() {
	//structure:
	/*
	 *  [ header(2) | angle(9) | scaler(10) | throttle(11) ]
	 */
	uint32_t final_packet = 0;
	//strip values to required number of bits:
	values[0] &= (0x01FF);
	values[1] &= (0x03FF);
	values[2] &= (0x07FF);
	//add em all into one packet
	final_packet = (values[0] << 21);
	final_packet |= (values[1] << 11);
	final_packet |= (values[2]);
	final_packet |= (2 << 30);	//header
	return final_packet;
}
