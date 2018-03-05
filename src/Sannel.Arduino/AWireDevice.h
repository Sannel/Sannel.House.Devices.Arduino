#ifndef _AWIREDEVICE_H_
#define _AWIREDEVICE_H_

#include "IWireDevice.h"
#include "Arduino.h"

using namespace Sannel::House::Sensor;

class AWireDevice : public IWireDevice
{
public:
	AWireDevice(uint8_t id);
	void Write(uint8_t b) override;
	void Write(uint8_t b1, uint8_t b2) override;
	uint8_t WriteRead(uint8_t write) override;
	void WriteRead(uint8_t write, uint8_t* read, int length) override;
};

#endif