#ifndef _AWIRE_H_
#define _AWIRE_H_

#include "IWire.h"
#include "IWireDevice.h"
#include "AWireDevice.h"

using namespace Sannel::House::Sensor;

class AWire : public IWire
{
public:
	AWire();
	IWireDevice& GetDeviceById(uint8_t deviceId) override;
};

#endif