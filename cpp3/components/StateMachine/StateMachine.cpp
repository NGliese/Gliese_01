/*
 * StateMachine.cpp
 *
 * Created: 07/12/2017 11.25.46
 *  Author: Gliese
 */


static char tag[]="StateMachine";


#include <StateMachine.h>
#include <StateMachine_Config.h>
#include <esp_log.h>
#include <string>
#include "sdkconfig.h"


#define PORT 22

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

State_Machine * sm = nullptr;



State_Machine::State_Machine(void) {
    current_event = nill_event;
    current_state = init_state;
    // initialize all
    capacity = 1;
    vsize = 0;
    content = new sm_pair[capacity];

    pWiFi = nullptr;
    pSocketServer = nullptr;

#if defined(CONFIG_BT_ENABLED)
    pServer = nullptr;
    pService = nullptr;
    pCharacteristic = nullptr;
    pAdvertising = nullptr;
#endif
}

State_Machine::~State_Machine(void) {

}

void State_Machine::initialize(void) {


}

void State_Machine::run(void) {


    /* 0) test for error_events */


    /*1) store event 0 from quarry */
    current_pair = at(0);
    /*2) remove event 0 from quarry */
    remove_from_quarry(0);
    /*3) fire stored event */
    eval(current_pair);
}



void State_Machine::eval(sm_pair p) {

	if (current_state == p.expected_state) {




		current_event = p.next_event;
		//determine the State-Matrix-Element in dependany of current state and triggered event
		StateElement stateEvaluation;
		stateEvaluation = StateMatrix[current_state][current_event];
		ESP_LOGD(tag, "[expected state: %s],[Current state: %s], -> [next event: %s],[next state: %s]", get_state(p.expected_state),get_state(current_state),get_event(p.next_event),get_state(stateEvaluation.nextState));


		//do the transition to the next state (set requestet next state to current state)...
		current_state = stateEvaluation.nextState;

		//... and fire the proper action
		(*stateEvaluation.actionToDo)();

	}

}

void State_Machine::quarry_add(sm_pair p) {
    // resize array if full
    if (vsize == capacity) {
        resize();
    }
    // insert new element
    content[vsize] = p;
    vsize++;
}

void State_Machine::print_quarry(void) {
}

void State_Machine::print_state(void) {

}

sm_pair State_Machine::at(unsigned int i) {
    // we check if we are accessing an element of the statemachine
    // otherwise we raise an error
    if (i >= vsize) {
    	sm_pair p ={nill_event,nill_state};
        return p; // error
    } else return content[i];
}

void State_Machine::remove_from_quarry(unsigned int x) {
    // we check if we are accessing an element of the statemachine
    // otherwise we raise an error
    if (x >= vsize) {
        return; // error
    } else {
        // save pointer to current array
        sm_pair * old_content = content;

        content = new sm_pair[capacity];
        // copy content
        int new_i = 0;
        for (unsigned int i = 0; i < vsize; i++) {
            if (i == x) {
                if (vsize > 0) {
                    // this element is ignored, therefore the vsize is -1
                }

            } else {
                content[new_i] = old_content[i];
                new_i++;
            }
       

        }
             vsize = new_i;
        // delete old array
        delete [] old_content;
    }


}

void State_Machine::resize(void) {
    // save pointer to current array
    sm_pair * old_content = content;

    // allocate new array: we try to double the current capacity
    capacity = capacity * 2;
    content = new sm_pair[capacity];

    // copy content
    for (unsigned int i = 0; i < vsize; i++) {
        content[i] = old_content[i];
    }

    // delete old array
    delete [] old_content;
}

const char * State_Machine::get_event(events e) {
    switch (e) {
        case error_event:
            return "error_event";
        case done_event:
            return "done_event";
        case ble_init_event:
            return "ble_init_event";
        case wifi_init_event:
            return "wifi_init_event";
        case idle_event:
            return "idle_event";
        case four_event:
            return "four_event";
        case nill_event:
            return "nill_event";

    }
    return "0";
}

const char * State_Machine::get_state(states s) {
    switch (s) {
        case nill_state:
            return "nill_state";
        case error_state:
            return "error_state";
        case init_state:
            return "init_state";
        case idle_state:
            return "idle_state";
        case BLE_init_state:
            return "BLE_init_state";
        case BLE_idle_state:
            return "BLE_idle_state";
        case wifi_init_state:
            return "wifi_init_state";
        case wifi_idle_state:
            return "wifi_idle_state";

    }
    return "0";
}


bool State_Machine::init_ble()
{
#if defined(CONFIG_BT_ENABLED)
	//
	BLEDevice::init("Bob");

	pServer =  BLEDevice:: createServer();

	pService = pServer->createService(BLEUUID(SERVICE_UUID));

	pCharacteristic = pService->createCharacteristic(
	BLEUUID(CHARACTERISTIC_UUID),
	BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
);
	pCharacteristic->setValue("Hello world, says bob");

	pService->start();
	pAdvertising = pServer->getAdvertising();
	pAdvertising->start();

#endif // CONFIG_BT_ENABLED
	return true;
}

bool State_Machine::init_wifi()
{
	nvs_flash_init();
	pSocketServer = new SockServ(PORT);
	pWiFi =  new WiFi("F1U3SMÆKK3R3N","Rododendron",pSocketServer);

	pWiFi->initialize_connection();
	return true;

}
bool State_Machine::init_wifi(std::string ssid, std::string pwd)
{
	nvs_flash_init();
	pSocketServer = new SockServ(PORT);
	pWiFi =  new WiFi(ssid,pwd,pSocketServer);
	pWiFi->initialize_connection();
	return true;
}


bool State_Machine::wifi_run()
{
	pWiFi->run();
	pSocketServer->sendData("Hello World");
	return true;
}

void set_state_machine(State_Machine* state_machine)
{
	sm = state_machine;
}

void zzz(void) {

}

void initialize_fcn(void) {
// initialize fcn
	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "a01");
	ESP_LOGD(tag, " ************************************");
 sm_pair p1;
 p1.expected_state = idle_state;
 //p1.next_event = wifi_init_event;
 p1.next_event = ble_init_event;
 sm->quarry_add(p1);

}

void ble_init_fcn(void) {
// bluetooth initialize
	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "a02");
	ESP_LOGD(tag, " ************************************");

	sm->init_ble();
	 sm_pair p1;
	 p1.expected_state = BLE_init_state;
	 p1.next_event = done_event;
	 sm->quarry_add(p1);


}

void wifi_init_fcn(void)
{
	// wifi initialize
	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "a03");
	ESP_LOGD(tag, " ************************************");
	// wifi initialize
	sm->init_wifi();
	sm_pair p1;
    p1.expected_state = wifi_init_state;
	p1.next_event = done_event;
	sm->quarry_add(p1);


}
void wifi_idle_fcn(void) {
	// wifi idle
	ESP_LOGD(tag, " ************************************");
	ESP_LOGD(tag, "a04");
	ESP_LOGD(tag, " ************************************");
	sm->wifi_run();
	sm_pair p1;
    p1.expected_state = wifi_idle_state;
	p1.next_event = idle_event;
	sm->quarry_add(p1);

}
