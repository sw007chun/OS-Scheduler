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
	case CREATED: return "CREATED";
	case READY: return "READY";
	case RUNNING: return "RUNNG";
	case BLOCKED: return "BLOCK";
	default: return "";
	}
}

void Simulation::startSim (Scheduler * sched, queue <Process *> *inputQ) {
	int *pSum[] = new int[inputQ->size()][10];
	int *totalSum;
	int CURRENT_TIME = 0;
	int timeInPrevState;
	bool CALL_SCHEDULER = false;
	queue <Event *> eQ;
	Process *proc; // = inputQ->front();
	Process *CURRENT_RUNNING_PROCESS = NULL;
	Event *evt; // = new Event(proc, CREATED, READY, proc->GetTimeStamp());
//	eQ.push(evt);
//	inputQ->pop();

	int cTime;

	while (!eQ.empty() || !inputQ->empty()) {
		if (eQ.empty() || (!inputQ->empty() &&  inputQ->front()->GetTimeStamp() <= eQ.front()->TimeStamp())) {
			evt = new Event(inputQ->front(), CREATED, READY, inputQ->front()->GetTimeStamp());
			inputQ->pop();
		} else {
			evt = eQ.front();
			eQ.pop();
		}

		proc = evt->evtProcess();
		CURRENT_TIME = evt->TimeStamp();
		timeInPrevState = CURRENT_TIME - proc->GetTimeStamp();

		cout << CURRENT_TIME << ' ' << proc->GetNum() << ' ' << timeInPrevState << ": " ;

		switch(evt->Transition()) {
		case READY:
			if (evt->GetPrevState() == BLOCKED)
				proc->AddIOTime(timeInPrevState);
//			eQ.push(new Event(proc, READY, RUNNING, CURRENT_TIME));
			CALL_SCHEDULER = true;
			cout << GetStateName(evt->GetPrevState()) << " -> READY " << endl;
			sched->push(proc);
			break;
		case RUNNING:
			cTime = myRandom(proc->GetCPUBurst());

			if (cTime >= proc->GetRem())
				eQ.push(new Event(proc, RUNNING, DONE, CURRENT_TIME + proc->GetRem()));
			else
				eQ.push(new Event(proc, RUNNING, BLOCKED, CURRENT_TIME + cTime));

			CURRENT_RUNNING_PROCESS = proc;
			CALL_SCHEDULER = false;

			cout <<"READY -> RUNNG cb=" << cTime << " rem=" << proc->GetRem() << " prio=" << proc->GetPrio() << endl;

			break;
		case BLOCKED:
			cTime = myRandom(proc->GetIOBurst());
			proc->AddCPUTime(timeInPrevState);
			eQ.push(new Event(proc, BLOCKED, READY, CURRENT_TIME + cTime));

			CURRENT_RUNNING_PROCESS = NULL;
			CALL_SCHEDULER = true;

			cout << "RUNNG -> BLOCK io=" << cTime << " rem=" << proc->GetRem() << endl;

			break;
		case PREEMPT:
			CALL_SCHEDULER = true;
			break;
		case DONE:
			CALL_SCHEDULER = true;
			CURRENT_RUNNING_PROCESS = NULL;
			cout << "DONE" << endl;
			break;
		}

		delete evt;
		evt = NULL;

		if (CALL_SCHEDULER) {
			if (!eQ.empty() && eQ.front()->TimeStamp() == CURRENT_TIME) {
				continue;
			}
			CALL_SCHEDULER = false;
//			cout << CURRENT_RUNNING_PROCESS << ' ' << eQ.empty() << endl;
			if (CURRENT_RUNNING_PROCESS == NULL && eQ.empty() ) {
				CURRENT_RUNNING_PROCESS = sched->get_next_process();
				if (CURRENT_RUNNING_PROCESS == NULL)
					continue;
				eQ.push(new Event(CURRENT_RUNNING_PROCESS, READY, RUNNING, CURRENT_TIME));
//				eQ.push(new Event(CURRENT_RUNNING_PROCESS, CREATED, READY, sched->CurrentProcess()->GetTimeStamp()));
			}
		}
	}


}
