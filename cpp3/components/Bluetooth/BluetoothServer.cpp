


#include <BluetoothServer.h>
#include <BluetoothConfig.h>






static char LOG_TAG[] = "SampleServer";
	void BluetoothServer:: run(void *data) {
		ESP_LOGD(LOG_TAG, "Starting BLE work!");

		BLEDevice::init("Bob");
		pServer = BLEDevice::createServer();

		pAdvertising = pServer->getAdvertising();
		inittialize();
		pAdvertising->start();




		ESP_LOGD(LOG_TAG, "Advertising started!");
		delay(1000);
	}

	std::string BluetoothServer::get_value(const char* serv_uuid,
			const char* char_uuid) {

		ESP_LOGD(LOG_TAG, " ************************************");
		ESP_LOGD(LOG_TAG, "get_value");

		for (unsigned int i = 0;  i < m_serviceUUIDs.size();  i++) {
			if(m_serviceUUIDs.at(i)->getUUID().equals(BLEUUID(serv_uuid)))
			{
				ESP_LOGD(LOG_TAG, " -> uuid match");
			BLECharacteristic* pChar =	m_serviceUUIDs.at(i)->getCharacteristic(BLEUUID(char_uuid));
			if(pChar != nullptr) return pChar->getValue();
			}
		}
		ESP_LOGD(LOG_TAG, " ************************************");

		return "0";
	}

	void BluetoothServer::set_value(const char* serv_uuid, const char* char_uuid,
			std::string data) {
		ESP_LOGD(LOG_TAG, " ************************************");
		ESP_LOGD(LOG_TAG, "set_value");

		for (unsigned int i = 0;  i < m_serviceUUIDs.size();  i++) {
			if(m_serviceUUIDs.at(i)->getUUID().equals(BLEUUID(serv_uuid)))
			{
				ESP_LOGD(LOG_TAG, " -> uuid match");
			BLECharacteristic* pChar =	m_serviceUUIDs.at(i)->getCharacteristic(BLEUUID(char_uuid));
			if(pChar != nullptr) pChar->setValue(data);
			}
		}
		ESP_LOGD(LOG_TAG, " ************************************");
	}

void BluetoothServer::inittialize()
{


/********************************************************************************************
 *							SERVICE: LIGHT
 * *****************************************************************************************/

	pService_light = pServer->createService(BLEUUID(SERVICE_LIGHT_UUID));

	pCharacteristic = pService_light->createCharacteristic(
		BLEUUID(CHAR_LIGHT_MEASURE),
		BLECharacteristic::PROPERTY_READ
	);
	pCharacteristic->setValue("Hello World, measure light");

	pCharacteristic = pService_light->createCharacteristic(
		BLEUUID(CHAR_LIGHT_ACTUATE),
	    BLECharacteristic::PROPERTY_WRITE
	);
	pCharacteristic->setValue("Hello World, actuate light");


	pService_light->start();


	pAdvertising->addServiceUUID(BLEUUID(pService_light->getUUID()));
	m_serviceUUIDs.push_back(pService_light);

	/********************************************************************************************
	 *						SERVICE: TEMP
	 * *****************************************************************************************/



	pService_temp = pServer->createService(BLEUUID(SERVICE_TEMP_UUID));

	pCharacteristic = pService_temp->createCharacteristic(
		BLEUUID(CHAR_TEMP_MEASURE),
		BLECharacteristic::PROPERTY_READ
	);
	pCharacteristic->setValue("Hello World, measure temp");


	pCharacteristic = pService_temp->createCharacteristic(
		BLEUUID(CHAR_TEMP_ACTUATE),
		BLECharacteristic::PROPERTY_WRITE
	);
	pCharacteristic->setValue("Hello World, actuate temp");

	pService_temp->start();
	pAdvertising->addServiceUUID(BLEUUID(pService_temp->getUUID()));
	m_serviceUUIDs.push_back(pService_temp);

	/********************************************************************************************
	 *						SERVICE: ALARM
	 * *****************************************************************************************/
	pService_alarm = pServer->createService(BLEUUID(SERVICE_ALARM_UUID));

	pCharacteristic = pService_alarm->createCharacteristic(
		BLEUUID(CHAR_ALARM_ACTUATE),
		BLECharacteristic::PROPERTY_WRITE
	);
	pCharacteristic->setValue("Hello World, actuate alarm");
	pService_alarm->start();
	pAdvertising->addServiceUUID(BLEUUID(pService_alarm->getUUID()));
	m_serviceUUIDs.push_back(pService_alarm);

	/********************************************************************************************
	 *
	 * *****************************************************************************************/


}



