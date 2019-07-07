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

void Simulation::startSim (Scheduler * sched) {
	int CURRENT_TIME = 0;
	int timeInPrevState;
	bool CALL_SCHEDULER = false;
	queue <Event *> eQ;
	Process *proc = sched->get_next_process();
//	Process *CURRENT_RUNNING_PROCESS = proc;
	Event *evt = new Event(proc, CREATED, READY, proc->GetTimeStamp());
	eQ.push(evt);

	int cTime;

	while (!eQ.empty()) {
		evt = eQ.front();
		eQ.pop();
		proc = evt->evtProcess();
		CURRENT_TIME = evt->TimeStamp();
		timeInPrevState = CURRENT_TIME - proc->GetTimeStamp();

		cout << CURRENT_TIME << ' ' << proc->GetNum() << ' ' << timeInPrevState << ": " ;

		switch(evt->Transition()) {
		case READY:
			if (evt->GetPrevState() == BLOCKED)
				proc->AddIOTime(timeInPrevState);
			eQ.push(new Event(proc, READY, RUNNING, CURRENT_TIME));
			sched->push(proc);
			CALL_SCHEDULER = true;
			cout << GetStateName(evt->GetPrevState()) << " -> READY " << endl;
			break;
		case RUNNING:
			cTime = myRandom(proc->GetCPUBurst());
			if (cTime >= proc->GetRem()) {
				cTime = proc->GetRem();
				eQ.push(new Event(proc, RUNNING, DONE, CURRENT_TIME + cTime));
			}
			else {
				eQ.push(new Event(proc, RUNNING, BLOCKED, CURRENT_TIME + cTime));
			}
			cout <<"READY -> RUNNG cb=" << cTime << " rem=" << proc->GetRem() << " prio=" << proc->GetPrio() << endl;
			sched->pop();
			CALL_SCHEDULER = false;
			break;
		case BLOCKED:
			cTime = myRandom(proc->GetIOBurst());
			proc->AddCPUTime(timeInPrevState);
			eQ.push(new Event(proc, BLOCKED, READY, CURRENT_TIME + cTime));
			CALL_SCHEDULER = true;
			cout << "RUNNG -> BLOCK io=" << cTime << " rem=" << proc->GetRem() << endl;
			break;
		case PREEMPT:
			CALL_SCHEDULER = true;
			break;
		case DONE:
			CALL_SCHEDULER = true;
			cout << "DONE" << endl;
//			sched->pop();
//			proc = sched->CurrentProcess();
//			cout << proc->GetNum() << endl;
			break;
		}
//		if (add == 1) {
//			cout << from << " -> " << to << " cb=" << cTime << " rem=" << proc->GetRem() << " prio=" << proc->GetPrio();
//		} else if (add == 2) {
//			cout << from << " -> " << to << " io=" << cTime << " rem=" << proc->GetRem();
//		} else if (add == 3) {
//			cout << "DONE" ;
//		}
//		cout << endl;
//		add = 0;

		delete evt;
		evt = NULL;

		if (CALL_SCHEDULER) {
			if (!eQ.empty() && eQ.front()->TimeStamp() == CURRENT_TIME) {
				continue;
			}
			CALL_SCHEDULER = false;

			if (sched->CurrentProcess() == NULL && eQ.empty() ) {
				sched->push(sched->get_next_process());
				if (sched->CurrentProcess() == NULL)
					continue;
				eQ.push(new Event(sched->CurrentProcess(), CREATED, READY, sched->CurrentProcess()->GetTimeStamp()));
			}
		}
	}
}
