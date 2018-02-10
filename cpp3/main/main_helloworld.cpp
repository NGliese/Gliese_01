/*
 * 1. Open up the project properties
 * 2. Visit C/C++ General > Preprocessor Include Paths, Macros, etc
 * 3. Select the Providers tab
 * 4. Check the box for "CDT GCC Built-in Compiler Settings"
 * 5. Set the compiler spec command to "xtensa-esp32-elf-gcc ${FLAGS} -E -P -v -dD "${INPUTS}""
 * 6. Rebuild the index
*/

#include <esp_log.h>
#include <string>
#include "sdkconfig.h"
#include <StateMachine.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include <WiFi.h>
#include <nvs_flash.h>
#include <Socket.h>
#include <SockServ.h>

#include "../components/Bluetooth_API/include/BLEDevice.h"
#define PORT 22

#define add "192.168.0.130"
static char tag[]="cpp_helloworld";



extern "C" {
	void app_main(void);
}

class Greeting {
public:
	void helloEnglish() {
		ESP_LOGD(tag, "Hello %s", name.c_str());
	}

	void helloFrench() {
		ESP_LOGD(tag, "Bonjour %s", name.c_str());
	}

	void setName(std::string name) {
		this->name = name;
	}
private:
	std::string name = "";

};



void app_main(void)
{
	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "app main start");
	ESP_LOGD(tag, " ************************************");

	//nvs_flash_init();

//	SockServ * ss = new SockServ(PORT);
	State_Machine * sm = new State_Machine();
	set_state_machine(sm);

//	WiFi * wifi = new WiFi("F1U3SMÆKK3R3N","Rododendron",ss);

	sm_pair p1;
		p1.expected_state = init_state;
		p1.next_event =done_event;

		sm->quarry_add(p1);
		//esp_task_wdt_init(2,0);

		ESP_LOGD(tag, " ************************************");
		ESP_LOGD(tag, "Bluetooth tag ");
		ESP_LOGD(tag, " ************************************");


//wifi->initialize_connection();
while(1)
{
	sm->run();

    //esp_task_wdt_feed(); //(1)
	// use in while loop
    vTaskDelay(10000); //(2)
//s->send("Hello world");
    // to send data:
  // ss->sendData("Hello world");

//ss->receiveData(s,data,30);
/*
printf("data :");
for (int i = 0; i<30; i++)
{
	printf("%c, ",data[i]);
}
*/
    //taskYIELD(); //(3)
}
}

