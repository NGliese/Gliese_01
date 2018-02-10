/*
 * BluetoothServer.h
 *
 *  Created on: Feb 10, 2018
 *      Author: gliese
 */

#ifndef COMPONENTS_BLUETOOTH_INCLUDE_BLUETOOTHSERVER_H_
#define COMPONENTS_BLUETOOTH_INCLUDE_BLUETOOTHSERVER_H_
#include <Task.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <esp_log.h>
#include <string>
#include <vector>
class BluetoothServer: public Task {
public :

	void run(void * data);

	// get value dependend on the uuid of the service and the uuid of the characteristic
	std::string get_value(const char* serv_uuid, const char* char_uuid);
	void set_value(const char* serv_uuid, const char* char_uuid,std::string data);
private:
	void inittialize(void);
	BLEServer* pServer;
	BLEAdvertising* pAdvertising;
	BLEService* pService_light;
	BLEService* pService_temp;
	BLEService* pService_alarm;
	BLECharacteristic* pCharacteristic;
	std::vector<BLEService*> m_serviceUUIDs;


};

#endif /* COMPONENTS_BLUETOOTH_INCLUDE_BLUETOOTHSERVER_H_ */
