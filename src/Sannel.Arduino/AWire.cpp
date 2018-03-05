#include "AWire.h"


AWire::AWire()
{
}

IWireDevice& AWire::GetDeviceById(uint8_t deviceId)
{
	return *(new AWireDevice(deviceId));
}