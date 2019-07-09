/*
 * Event.h
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "Process.h"

typedef enum {CREATED, READY, RUNNING, BLOCKED, PREEMPT, DONE} pstates;

class Event {
private:
	Process * evtProc;
	int oldState;
	int newState;
	int evt_ts;
public:
	Event(Process * p, int s, int time, int trans);
	Process * evtProcess();
	int GetTimeStamp();
	int Transition();
	int GetPrevState();
};



#endif /* EVENT_H_ */
