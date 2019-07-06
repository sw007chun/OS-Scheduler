/*
 * Event.cpp
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef EVENT_CPP_
#define EVENT_CPP_

#include "Event.h"

Event::Event(Process * p, int s, int time, int trans) {
	state = s;
	evt_ts = time;
	evtProc = p;
	transition = trans;
}

Process * Event::evtProcess() {
	return evtProc;
}

int Event::TimeStamp() {
	return evt_ts;
}

int Event::Transition() {
	return transition;
}

int Event::GetState() {
	return state;
}

#endif /* EVENT_CPP_ */
