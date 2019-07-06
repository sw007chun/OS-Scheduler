/*
 * Simulation.cpp
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#include "Simulation.h"
#include "Scheduler.h"
#include "Event.h"

Simulation::Simulation(int * rand) {
	randvals = rand;
	ofs = 0;
}

int Simulation::myRandom (int burst) {
	return 1 + (randvals[ofs++] % burst);
}

const char * GetStateName (int state) {
	switch (state) {
	case STATE_CREATED: return "CREATED";
	case STATE_READY: return "READY";
	case STATE_RUNNING: return "RUNNG";
	case STATE_BLOCKED: return "BLOCK";
	default: return "";
	}
}

void Simulation::startSim (Scheduler * sched) {
	int CURRENT_TIME = 0;
	int timeInPrevState;
	string from, to;
	bool CALL_SCHEDULER = false;
	bool add = false;
	queue <Event *> eQ;
	Event *evt = new Event(sched->get_next_process(), STATE_CREATED, 0, TRANS_TO_READY);
	eQ.push(evt);

	while (!eQ.empty()) {
		evt = eQ.front();
		eQ.pop();
		Process *proc = evt->evtProcess();
		CURRENT_TIME = evt->TimeStamp();
		timeInPrevState = CURRENT_TIME - proc->TimeStamp();
		from = GetStateName(evt->GetState());

		switch(evt->Transition()) {
		case TRANS_TO_READY:
			eQ.push(new Event(proc, STATE_READY, CURRENT_TIME, TRANS_TO_RUN));
			sched->push(proc);
			CALL_SCHEDULER = true;
			add = true;
			to = "READY";
			break;
		case TRANS_TO_RUN:
			eQ.push(new Event(proc, STATE_RUNNING, CURRENT_TIME + myRandom(proc->GetCPUBurst()), TRANS_TO_BLOCK));
			to = "RUNNG";
			add = true;
			break;
		case TRANS_TO_BLOCK:
			eQ.push(new Event(proc, STATE_BLOCKED, CURRENT_TIME + myRandom(proc->GetIOBurst()), TRANS_TO_READY));
			CALL_SCHEDULER = true;
			to = "BLOCK";
			break;
		case TRANS_TO_PREEMPT:
			CALL_SCHEDULER = true;
			to = "PREMPT";
			break;
		}
		cout << CURRENT_TIME << ' ' << proc->GetNum() << ' ' << timeInPrevState << ": " << from << " -> " << to ;
		if (add) {
			cout << "cb=" << proc->GetCPUBurst() ;
		}
		cout << endl;

		delete evt;
		evt = NULL;

		if (CALL_SCHEDULER) {
			if (eQ.front()->TimeStamp() == CURRENT_TIME) {
				continue;
			}
			CALL_SCHEDULER = false;
		}

	}
}
