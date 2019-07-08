/*
 * Simulation.cpp
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#include "Simulation.h"
#include "Scheduler.h"
#include "Event.h"
#include <iomanip>

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
	int inputSize = inputQ->size();
	Process **pSum = new Process*[inputSize]; //summary
	double totalSum[5];
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

			if (cTime >= proc->GetRem()) {
				cTime = proc->GetRem();
				eQ.push(new Event(proc, RUNNING, DONE, CURRENT_TIME + proc->GetRem()));
			}
			else
				eQ.push(new Event(proc, RUNNING, BLOCKED, CURRENT_TIME + cTime));

			CURRENT_RUNNING_PROCESS = proc;
			CALL_SCHEDULER = false;

			cout <<"READY -> RUNNG cb=" << cTime << " rem=" << proc->GetRem() << " prio=" << proc->GetDynPrio() << endl;

			break;
		case BLOCKED:
			cTime = myRandom(proc->GetIOBurst());
			proc->AddCPUTime(timeInPrevState);
			eQ.push(new Event(proc, BLOCKED, READY, CURRENT_TIME + cTime));

			CURRENT_RUNNING_PROCESS = NULL;
			CALL_SCHEDULER = true;

			cout << "RUNNG -> BLOCK  ib=" << cTime << " rem=" << proc->GetRem() << endl;

			break;
		case PREEMPT:
			CALL_SCHEDULER = true;
			break;
		case DONE:
			proc->AddCPUTime(timeInPrevState);
			CALL_SCHEDULER = true;
			CURRENT_RUNNING_PROCESS = NULL;
//			int pid = proc->GetNum();
			pSum[proc->GetNum()] = proc;
			cout << "Done" << endl;
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
			if (CURRENT_RUNNING_PROCESS == NULL ) {
				CURRENT_RUNNING_PROCESS = sched->get_next_process();
				if (CURRENT_RUNNING_PROCESS == NULL)
					continue;
				eQ.push(new Event(CURRENT_RUNNING_PROCESS, READY, RUNNING, CURRENT_TIME));
//				eQ.push(new Event(CURRENT_RUNNING_PROCESS, CREATED, READY, sched->CurrentProcess()->GetTimeStamp()));
			}
		}
	}

//	totalSum[0] = CURRENT_TIME;
	cout << sched->Get_SType() << endl;
	for (int i = 0; i < inputSize ; i++ ) {
		cout << setw(4) << setfill('0') << pSum[i]->GetNum() <<":";
		cout << setw(5) <<  setfill(' ') << pSum[i]->GetArrivalTime();
		cout << setw(5) << pSum[i]->GetTotalCPU();
		cout << setw(5) << pSum[i]->GetCPUBurst();
		cout << setw(5) << pSum[i]->GetIOBurst();
		cout << setw(2) << pSum[i]->GetStatPrio() << " |";
		cout << setw(6) << pSum[i]->GetTimeStamp();
		cout << setw(6) << pSum[i]->GetTimeStamp() - pSum[i]->GetArrivalTime();
		cout << setw(6) << pSum[i]->GetTotalIO();
		cout << setw(6) << pSum[i]->GetTimeStamp() - pSum[i]->GetArrivalTime() - pSum[i]->GetTotalIO() - pSum[i]->GetTotalCPU() << endl;
		totalSum[0] += pSum[i]->GetTotalCPU();
		totalSum[1] += pSum[i]->GetTotalIO();
		totalSum[2] += pSum[i]->GetTimeStamp() - pSum[i]->GetArrivalTime();
		totalSum[3] += pSum[i]->GetTimeStamp() - pSum[i]->GetArrivalTime() - pSum[i]->GetTotalIO() - pSum[i]->GetTotalCPU();
	}
	totalSum[4] = inputSize;
	cout << "SUM: " << CURRENT_TIME ;

	for (int i = 0; i < 5; i++) {
		if (i == 2)
			cout << setprecision(2) << fixed << ' ' << totalSum[i] / inputSize;
		else if (i == 4)
			cout << setprecision(3) << fixed << ' ' << totalSum[i] / CURRENT_TIME * 100;
		else
			cout << setprecision(2) << fixed << ' ' << totalSum[i] / CURRENT_TIME * 100;
	}
	cout << endl;
}
