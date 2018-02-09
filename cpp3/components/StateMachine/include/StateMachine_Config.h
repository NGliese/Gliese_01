/*
* StateMachine_Config.h
*
* Created: 08/12/2017 09.02.00
*  Author: Gliese
*/


#ifndef STATEMACHINE_CONFIG_H_
#define STATEMACHINE_CONFIG_H_

#include "StateMachine.h"

typedef void (*Action)();
typedef struct {
	states nextState; // Enumerator for the next state
	Action actionToDo; // function-pointer to the action that shall be released in current state
} StateElement; // structure for the elements in the state-event matrix
// And finally the state-event matrix will be build-up:
//Actions

void zzz(void);
void initialize_fcn(void);
void ble_init_fcn(void);
void wifi_init_fcn(void);
void wifi_idle_fcn(void);

static const StateElement StateMatrix[8][6] =
{
	//   error_event			nill_event				done_event			       		 ble_init_event					wifi_init_event				idle_event
	{ { nill_state,zzz}, 	   { nill_state,zzz},      { nill_state,zzz}  ,         	{ nill_state,zzz}  ,     	   { nill_state,zzz}   ,     		{ nill_state,zzz}    			}, // nill_state
	{ { error_state,zzz}	,  { error_state,zzz},     { error_state,zzz} ,         	{ error_state,zzz},      	   { error_state,zzz}  ,     		{ error_state,zzz}   			}, // error_state
	{ { init_state,zzz}, 	   { init_state,zzz},      { idle_state,initialize_fcn},	{ init_state,zzz} ,      	   { init_state,zzz}   ,     		{ init_state,zzz}    			}, // init_state
	{ { idle_state,zzz},  	   { idle_state,zzz},      { idle_state,zzz} ,          	{ BLE_init_state,ble_init_fcn},{ wifi_init_state,wifi_init_fcn},{ wifi_init_state,wifi_init_fcn}}, // idle_state
	{ { BLE_init_state,zzz},   { BLE_init_state,zzz},  { BLE_idle_state,zzz} ,	    	{ BLE_init_state,zzz} ,  	   { BLE_init_state,zzz} ,   		{ BLE_init_state,zzz}			}, // BLE_init_state
	{ { BLE_idle_state,zzz},   { BLE_idle_state,zzz},  { BLE_idle_state,zzz} ,	    	{ BLE_idle_state,zzz} ,  	   { BLE_idle_state,zzz}  ,  		{ BLE_idle_state,zzz} 			}, // BLE_init_state
	{ { wifi_init_state,zzz},  { wifi_init_state,zzz}, { wifi_idle_state,wifi_idle_fcn},{ wifi_init_state,zzz} , 	   { wifi_init_state,zzz},   		{ BLE_idle_state,zzz} 			}, // wifi_init_state
	{ { wifi_idle_state,zzz},  { wifi_idle_state,zzz}, { wifi_idle_state,zzz},			{ wifi_idle_state,zzz} , 	   { wifi_idle_state,zzz},   	    { wifi_idle_state,wifi_idle_fcn}}, // wifi_idle_state
};



#endif /* STATEMACHINE_CONFIG_H_ */
