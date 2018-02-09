/*
 * WiFi.h
 *
 *  Created on: Jan 25, 2018
 *      Author: gliese
 */

#ifndef COMPONENTS_WIFI_INCLUDE_WIFI_H_
#define COMPONENTS_WIFI_INCLUDE_WIFI_H_
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <lwip/sockets.h>
#include <lwip/def.h>
#include <lwip/inet.h>
#include "sdkconfig.h"
#include <string>

#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <nvs_flash.h>
#include <Socket.h>
#include <SockServ.h>

typedef struct {
	bool eventFlag = false;
	//bool adcFlag = false;
} WiFi_Flags;

class WiFi {
public:
	WiFi(std::string ssid, std::string pwd, SockServ * ss);
	WiFi(std::string ssid, std::string pwd, Socket * s);
	~WiFi(void);


	void initialize_connection(void);
	void run(void);
	void send_msg(void);
protected:

private:
	wifi_init_config_t config;
	void scan_network(void);
	void establish_connection(wifi_ap_record_t & record);
	bool scan_results(void);

	void set_current(system_event_id_t & event_id,
			system_event_info_t & event_info);
	system_event_id_t * event_id;
	system_event_info_t * event_info;
	//wifi_scan_config_t scanConf;
	wifi_config_t staConfig;
	std::string ssid ;
	std::string pwd ;
	SockServ * ss;
	Socket * s;
};

#endif /* COMPONENTS_WIFI_INCLUDE_WIFI_H_ */
