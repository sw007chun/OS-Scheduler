/*
 * Event.cpp
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef EVENT_CPP_
#define EVENT_CPP_

#include "Event.h"

Event::Event(Process * p, int os, int ns, int time) {
	oldState = os;
	newState = ns;
	evt_ts = time;
	evtProc = p;
}
Process * Event::evtProcess() {
	return evtProc;
}
int Event::GetTimeStamp() {
	return evt_ts;
}
int Event::Transition() {
	return newState;
}
int Event::GetPrevState() {
	return oldState;
}

#endif /* EVENT_CPP_ */
