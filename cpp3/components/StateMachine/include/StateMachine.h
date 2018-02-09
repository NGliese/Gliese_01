/*
 * StateMachine.h
 *
 * Created: 07/12/2017 11.25.34
 *  Author: Gliese
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <BLEDevice.h>
#include <string>
#include <nvs_flash.h>
#include <Socket.h>
#include <SockServ.h>
#include <WiFi.h>
#include <BLE2902.h>

typedef enum {
	success, fail
} sm_msg;

typedef enum {
	error_event,
	nill_event, // null event
	done_event,
	ble_init_event,
	wifi_init_event,
	idle_event,
	four_event
} events;

typedef enum {
	nill_state,
	error_state,
	init_state,
	idle_state,
	BLE_init_state,
	BLE_idle_state,
	wifi_init_state,
	wifi_idle_state,

} states;

typedef struct {
	events next_event;
	states expected_state;
}sm_pair;


class State_Machine {
public:
	State_Machine(void);
	~State_Machine(void);

	//void quarry_add(events e);
	void quarry_add(sm_pair p);
	void run(void);
	bool wifi_run(void);
	/** debug functions**/
	void print_state(void);
	void print_quarry(void);

	// not ment to be called
	bool init_ble(void);
	bool init_wifi(void);
	bool init_wifi(std::string ssid, std::string pwd);
	/***/
protected:

private:
	/* private functions*/
	sm_pair at(unsigned int);
	void resize(void);
	void remove_from_quarry(unsigned int);
	const char * get_event(events e);
	const char * get_state(states s);
	void initialize(void);
	void eval(sm_pair p);
	/* Private vars*/

	events current_event;
	states current_state;
	sm_pair current_pair;
	sm_pair * content;
	unsigned int vsize;
	unsigned int capacity;
#if defined(CONFIG_BT_ENABLED)
	// bluetooth variables
	BLEServer *pServer;
	BLEService *pService;
	BLECharacteristic *pCharacteristic;
	BLEAdvertising *pAdvertising;
	BLE2902* p2902Descriptor;
#endif
	// wifi variables
	WiFi * pWiFi;
	SockServ *pSocketServer;

};

// workaround to parse the state machine from c++ to c code
void set_state_machine(State_Machine* state_machine);
#endif /* STATEMACHINE_H_ */
