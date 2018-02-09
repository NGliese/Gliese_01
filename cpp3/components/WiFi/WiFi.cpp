/*
 * WiFi.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: gliese
 */

#include <WiFi.h>
#include <lwip/sockets.h>
#include <esp_task_wdt.h>

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <mdns.h>

#include <freertos/FreeRTOS.h>
#include <nvs_flash.h>
#include <lwip/dns.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>

#include <string.h>

#include <esp_event.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include "sdkconfig.h"

#include <string>
#include <vector>
#include <mdns.h>
#include <esp_err.h>
#include <Socket.h>
#include <SockServ.h>
#define PORT 22


extern "C" {
	extern uint16_t lwip_ntohs(uint16_t);
	extern uint32_t lwip_ntohl(uint32_t);
	extern uint16_t lwip_htons(uint16_t);
	extern uint32_t lwip_htonl(uint32_t);
}


/*
 * Local function to handle events called from the ESP32 WiFi part
 * */
esp_err_t eventHandler(void* ctx, system_event_t* event);

system_event_t * current_event = nullptr;
static char tag[] = "wifi_module";
WiFi_Flags flags;


//uint8_t ssid[32] = "HUAWEI_P9_6856";
//uint8_t pwd[64] = "2sacredniko";

// the IP adress that we want our device to have, last 3 0.xxx can change
//#define DEVICE_IP 			"192.168.0.50"
// default gateway, to send data to
//#define Device_GW 			"192.168.0.1"
// The netmask specification (?)
//#define Device_NETMASK 		"255.255.255.0"

// The IP address that we want our device to have.
#define DEVICE_IP          "192.168.0.51"

// The Gateway address where we wish to send packets.
// This will commonly be our access point.
#define DEVICE_GW          "192.168.0.1"

// The netmask specification.
#define DEVICE_NETMASK     "255.255.255.0"

WiFi::WiFi(std::string ssid,std::string pwd, SockServ * ss) {
	this->s = nullptr;
	this->ss = ss;
	this->ssid = ssid;
	this->pwd = pwd;
	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "wifi init ");
	ESP_LOGD(tag, " ************************************");

	// create static ip
	tcpip_adapter_init();

	tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA); // Don't run a DHCP client
	tcpip_adapter_ip_info_t ipInfo;
	inet_pton(AF_INET, DEVICE_IP, &ipInfo.ip);
	inet_pton(AF_INET, DEVICE_GW, &ipInfo.gw);
	inet_pton(AF_INET, DEVICE_NETMASK, &ipInfo.netmask);
	tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);

	// add callback function
	ESP_ERROR_CHECK(esp_event_loop_init(eventHandler, NULL));

	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "wifi init ");
	ESP_LOGD(tag, " ************************************");

	//config = WIFI_INIT_CONFIG_DEFAULT();
	wifi_init_config_t con = WIFI_INIT_CONFIG_DEFAULT()
	;
	// initialize with default config
	ESP_ERROR_CHECK(esp_wifi_init(&con));

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	// stationary mode -> can connect to wifi router

	event_id = nullptr;
	event_info = nullptr;


	ESP_ERROR_CHECK(esp_wifi_start());

}

WiFi::WiFi(std::string ssid,std::string pwd, Socket * s) {
	this->s = s;
	this->ss = nullptr;
	this->ssid = ssid;
	this->pwd = pwd;
	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "wifi init ");
	ESP_LOGD(tag, " ************************************");

	// create static ip
	tcpip_adapter_init();

	tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA); // Don't run a DHCP client
	tcpip_adapter_ip_info_t ipInfo;
	inet_pton(AF_INET, DEVICE_IP, &ipInfo.ip);
	inet_pton(AF_INET, DEVICE_GW, &ipInfo.gw);
	inet_pton(AF_INET, DEVICE_NETMASK, &ipInfo.netmask);
	tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);

	// add callback function
	ESP_ERROR_CHECK(esp_event_loop_init(eventHandler, NULL));

	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "wifi init ");
	ESP_LOGD(tag, " ************************************");

	//config = WIFI_INIT_CONFIG_DEFAULT();
	wifi_init_config_t con = WIFI_INIT_CONFIG_DEFAULT()
	;
	// initialize with default config
	ESP_ERROR_CHECK(esp_wifi_init(&con));

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	// stationary mode -> can connect to wifi router

	event_id = nullptr;
	event_info = nullptr;


	ESP_ERROR_CHECK(esp_wifi_start());

}

WiFi::~WiFi() {

}

 void WiFi::send_msg(void) {
 // debug msg
 //ESP_LOGI("WiFi", "Protocol sent: ");
/*
 int socket_fd = socket(AF_INET,SOCK_DGRAM,0);
 std::string data = "Hello World, im new";
 const char d[] = "hello world";
 unsigned int size = data.size();



 std::string destAddr = "192.168.0.51";


 struct sockaddr_in serverAddress;
 serverAddress.sin_family = AF_INET;
 inet_pton(AF_INET, "192.168.1.200", &serverAddress.sin_addr.s_addr);
 serverAddress.sin_port = htons(22);



 //sendto(socket_fd,	d,	size,	0,	(struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)	);

*/

 }
/*
 Protocol* WiFi::recieve_msg(void) {

 return nullptr;
 }*/

/*
 * used to initialize connection to a (static) ssid network
 *  (easy to reconfig to parse ssid and pwd in fcn )
 * */

void WiFi::initialize_connection(void) {

	// initialize the ssid to '\0'
	for (unsigned int i = 0; i < sizeof(staConfig.sta.ssid); i++) {
		staConfig.sta.ssid[i] = '\0';
	}
	// initialize the password to '\0'
	for (unsigned int i = 0; i < sizeof(staConfig.sta.password); i++) {
		staConfig.sta.password[i] = '\0';
	}

	for (unsigned int i = 0; i < sizeof(ssid); i++) {
		staConfig.sta.ssid[i] = ssid[i];
	}
	for (unsigned int i = 0; i < sizeof(pwd); i++) {
		staConfig.sta.password[i] = pwd[i];
	}

	scan_network();
}

/**
 * scan network to determine the SSID available
 * */
void WiFi::scan_network(void) {
	wifi_scan_config_t scanConf;
	// create basic config file, not tested yet
	scanConf.ssid = nullptr;
	scanConf.bssid = nullptr;
	scanConf.channel = 0;
	scanConf.show_hidden = true;
	scanConf.scan_time.active.max = 250;
	scanConf.scan_time.active.min = 5;
	scanConf.scan_time.passive = 100;
	scanConf.scan_type = WIFI_SCAN_TYPE_ACTIVE;
	esp_wifi_scan_start(&scanConf, 0);

}

void WiFi::run(void) {

	if (flags.eventFlag) {

		switch (current_event->event_id) {
		case SYSTEM_EVENT_WIFI_READY:

			break;
		case SYSTEM_EVENT_SCAN_DONE:
			// the scanning of wifi network is done, scan_done data can be accessed
			ESP_LOGE(tag, "Scan done ")
			;
			if (scan_results()) {
				ESP_LOGD(tag, "Network found");
				// set the config parameters

				wifi_config_t sta_config;
				::memset(&sta_config, 0, sizeof(sta_config));
				::memcpy(sta_config.sta.ssid, ssid.data(), ssid.size());
				::memcpy(sta_config.sta.password, pwd.data(), pwd.size());
				sta_config.sta.bssid_set = 0;
				ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));


				ESP_ERROR_CHECK(esp_wifi_connect());
			}

			break;
		case SYSTEM_EVENT_STA_START:

			break;
		case SYSTEM_EVENT_STA_STOP:

			break;
		case SYSTEM_EVENT_STA_CONNECTED:
			ESP_LOGD(tag, "connected")
			;
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:

			break;
		case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:

			break;
		case SYSTEM_EVENT_STA_GOT_IP:
			//s->accept();
			//s->bind(20,(uint32_t)"192.168.0.130");
		ss->start();
		//	s->listen(22,0,0);
			//vTaskDelay(5000);
			//s->connect("192.168.0.130",22);

		//
		//	ss->sendData("Hello world");
			//ss->stop();
			break;
		case SYSTEM_EVENT_STA_LOST_IP:

			break;
		case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:

			break;
		case SYSTEM_EVENT_STA_WPS_ER_FAILED:

			break;
		case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:

			break;
		case SYSTEM_EVENT_STA_WPS_ER_PIN:

			break;
		case SYSTEM_EVENT_AP_START:

			break;
		case SYSTEM_EVENT_AP_STOP:

			break;
		case SYSTEM_EVENT_AP_STACONNECTED:

			break;
		case SYSTEM_EVENT_AP_STADISCONNECTED:

			break;
		case SYSTEM_EVENT_AP_PROBEREQRECVED:

			break;
		case SYSTEM_EVENT_GOT_IP6:

			break;
		case SYSTEM_EVENT_ETH_START:

			break;
		case SYSTEM_EVENT_ETH_STOP:

			break;
		case SYSTEM_EVENT_ETH_CONNECTED:

			break;
		case SYSTEM_EVENT_ETH_DISCONNECTED:

			break;
		case SYSTEM_EVENT_ETH_GOT_IP:

			break;
		case SYSTEM_EVENT_MAX:

			break;
		default:
			break;
		}

		// reset flags
		flags.eventFlag = false;
		// reset event
		current_event = nullptr;

	}
}

esp_err_t eventHandler(void* ctx, system_event_t* event) {
	// Handle events here
	current_event = event;

	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "wifi event ");
	ESP_LOGD(tag, " ************************************");
	flags.eventFlag = true;
	return ESP_OK;
}

/*
 * returns true if the ssid is found
 * */
bool WiFi::scan_results() {
	uint16_t apCount = current_event->event_info.scan_done.number;
	bool bssid = false;
	printf("Number of access points found: %d\n", apCount);
	if (apCount == 0) {
		return bssid;
	}
	uint16_t maximum = 10;
	wifi_ap_record_t list[maximum];

	uint16_t *num = &maximum;
	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(num, list));


	for (unsigned int i = 0; i < apCount; i++) {
		std::string authmode;
		switch (list[i].authmode) {
		case WIFI_AUTH_OPEN:
			authmode = "WIFI_AUTH_OPEN";
			break;
		case WIFI_AUTH_WEP:
			authmode = "WIFI_AUTH_WEP";
			break;
		case WIFI_AUTH_WPA_PSK:
			authmode = "WIFI_AUTH_WPA_PSK";
			break;
		case WIFI_AUTH_WPA2_PSK:
			authmode = "WIFI_AUTH_WPA2_PSK";
			break;
		case WIFI_AUTH_WPA_WPA2_PSK:
			authmode = "WIFI_AUTH_WPA_WPA2_PSK";
			break;
		case WIFI_AUTH_WPA2_ENTERPRISE:
			authmode = "WIFI_AUTH_WPA2_ENTERPRISE";
			break;
		default:
			authmode = "UNKNOWN";
			break;
		}

		printf("ssid=%s, rssi=%d,authmode=%s\n", list[i].ssid, list[i].rssi,
				authmode.c_str());

		// check for correct wifi_name (ssid)
		bool wifi_name = true;
		for (unsigned int n = 0; n < ssid.size(); n++) {
			if (list[i].ssid[n] != ssid.data()[n]) {
				wifi_name = false;
			}
			//	printf("list.ssid[n] = %c, config.ssid[n] = %c \n",list[i].ssid[n],staConfig.sta.ssid[n]);

		}
		// if the wifi_name has been identified, return true at the end.
		if (wifi_name) {
			bssid = true;
			ESP_LOGD(tag, " ************************************");
			ESP_LOGD(tag, "Wifi name found");
			ESP_LOGD(tag, " ************************************");
		}

	}

	//free(list);
	return bssid;

}

/*
 * unused
 * */
void WiFi::set_current(system_event_id_t& event_id,
		system_event_info_t& event_info) {

}
