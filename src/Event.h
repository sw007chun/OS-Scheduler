/*
 * Event.h
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "Process.h"

typedef enum {TRANS_CREATED, TRANS_TO_READY, TRANS_TO_RUN, TRANS_TO_BLOCK, TRANS_TO_PREEMPT} state_trans;

class Event {
private:
	Process * evtProc;
	int state;
	int evt_ts;
	int transition;
public:
	Event(Process * p, int s, int time, int trans);
	Process * evtProcess();
	int TimeStamp();
	int Transition();
	int GetState();
};



#endif /* EVENT_H_ */
