/* Copyright 2017 Sannel Software, L.L.C.

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.*/

#define DEBUG

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <Wire.h>
#include <SannelDefines.h>
#include "LocalDefines.h"
#include <SensorStore.h>
#include <BME280.h>
#include <TMP102.h>
#include <HardwareSerial.h>
#include <SensorPacket.h>
#include <Si7021.h>

using namespace Sannel::House::Sensor;

WiFiClient client;
SensorStore store(2);
SensorPacket packet;
Temp::BME280 bme280_1(0x77);
Temp::BME280 bme280_2(0x76);
Temp::TMP102 temp(0x48);
Temp::Si7021 temp2(0x40);


void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	Serial.begin(115200);
	delay(250);
	Serial.println();
	Serial.println();
#ifdef DEBUG
	Serial.println("Debug");
#endif
	Serial.print("Connecting to ");
	Serial.println(SSID);
	WiFi.begin(SSID, SSIDPWD);
	while (WiFi.status() != WL_CONNECTED)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(250);
		digitalWrite(LED_BUILTIN, LOW);
		delay(100);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(150);
		Serial.print(".");
	}

	digitalWrite(LED_BUILTIN, LOW);
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	uint8_t mac[6];
	WiFi.macAddress(mac);

	store.SetMacAddress(mac);

	bme280_1.Begin();
	bme280_2.Begin();
	temp.Begin();
	temp.wakeup();
	temp2.Begin();
}

void loop()
{
	store.AddReading(bme280_1);
	ResetSensorPacket(packet);

	packet.SensorType = SensorTypes::Test;
	packet.Offset = 0;
	packet.Values[0] = bme280_1.GetTemperatureCelsius();
	packet.Values[1] = bme280_2.GetTemperatureCelsius();
	packet.Values[2] = temp.GetTemperatureCelsius();
	packet.Values[3] = temp2.GetRelativeHumidity();
	packet.Values[4] = temp2.GetTemperatureCelsius();

	Serial.print("Temp1:");
	Serial.println(packet.Values[0]);
	Serial.print("Temp2:");
	Serial.println(packet.Values[1]);

	store.AddReading(packet);

	if (client.connect(destination, SENSOR_BROADCAST_PORT))
	{
		Serial.println("Connected writing packets");
		store.WritePackets(client);
		client.flush();
		client.stop();
		store.Reset();
		Serial.println("packets written");
	}
	else
	{
		Serial.println("Unable to connect to remote computer");
		Serial.println(client.status());
	}

	delay(60000);
}
