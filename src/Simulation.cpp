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

void orderedPush(list <Event *> *eQ, Event *evt ) {
	list <Event *>::iterator it;
	for (it = eQ->begin() ; it != eQ->end(); it++) {
		if ( (*it)->GetTimeStamp() > evt->GetTimeStamp() ) {
			eQ->insert(it, evt);
			return;
		}
	}
	eQ->insert(it, evt);
}
void removeFuture(list <Event *> *eQ, Process *p ) {
	list <Event *>::iterator it;
	for (it = eQ->begin() ; it != eQ->end(); it++) {
		if ( (*it)->evtProcess()->GetNum() == p->GetNum() ) {
			eQ->erase(it);
			return;
		}
	}
}
int getFuture(list <Event *> *eQ, Process *p ) {
	list <Event *>::iterator it;
	for (it = eQ->begin() ; it != eQ->end(); it++) {
		if ( (*it)->evtProcess()->GetNum() == p->GetNum() ) {
			return (*it)->GetTimeStamp();
		}
	}
	return -1;
}

void Simulation::startSim (Scheduler * sched, queue <Process *> *inputQ) {
	int inputSize = inputQ->size();
	Process **pSum = new Process*[inputSize]; //summary
	double totalSum[5] = {0.0};
	int CURRENT_TIME = 0;
	int timeInPrevState;
	bool CALL_SCHEDULER = false;
	int ioState = 0, ioStart_ts = 0, totalIO = 0; //counters for at least one IO running time
	list <Event *> eQ;
	Process *proc;
	Process *CURRENT_RUNNING_PROCESS = NULL;
	Event *evt; // current event
	Event *nEvt; // newly added Event

	int cTime;

	while (!eQ.empty() || !inputQ->empty()) {
		if (eQ.empty() || (!inputQ->empty() &&  inputQ->front()->GetTimeStamp() <= eQ.front()->GetTimeStamp())) {
			evt = new Event(inputQ->front(), CREATED, READY, inputQ->front()->GetTimeStamp());
			inputQ->pop();
		} else {
			evt = eQ.front();
			eQ.pop_front();
		}

		proc = evt->evtProcess();
		CURRENT_TIME = evt->GetTimeStamp();
		timeInPrevState = CURRENT_TIME - proc->GetTimeStamp();
		if (evt->GetPrevState() != PREEMPT)
			cout << CURRENT_TIME << ' ' << proc->GetNum() << ' ' << timeInPrevState << ": " ;

  		switch(evt->Transition()) {
		case READY:
			if (evt->GetPrevState() == BLOCKED) {
				proc->Reset_Prio();
				proc->AddIOTime(timeInPrevState);
				ioState--;
				if (ioState == 0)
					totalIO += CURRENT_TIME - ioStart_ts;
			}

			CALL_SCHEDULER = true;
			sched->add_to_queue(proc);

			if (evt->GetPrevState() != PREEMPT) {
				cout << GetStateName(evt->GetPrevState()) << " -> READY " << endl;
				if (sched->Get_SType() == "PREPRIO" && CURRENT_RUNNING_PROCESS != NULL) {
					int compPrio = proc->GetDynPrio() > CURRENT_RUNNING_PROCESS->GetDynPrio();
					int compTime = getFuture(&eQ, CURRENT_RUNNING_PROCESS) > CURRENT_TIME;
					cout << "---> PRIO preemption "<< CURRENT_RUNNING_PROCESS->GetNum() << " by " << proc->GetNum() << " ? ";
					cout << compPrio << " TS=" << getFuture(&eQ, CURRENT_RUNNING_PROCESS) << " now=" << CURRENT_TIME << ") --> ";
					if (compPrio && compTime) {
						cout << "YES" << endl;
//						cout << "Erased: " <<  eQ.front()->evtProcess()->GetNum() << ' ' << eQ.front()->GetPrevState() <<' ' <<eQ.front()->GetTimeStamp()<< ' ' << endl;
						removeFuture(&eQ, CURRENT_RUNNING_PROCESS);
						nEvt = new Event(CURRENT_RUNNING_PROCESS, RUNNING, PREEMPT,CURRENT_TIME);
						orderedPush(&eQ, nEvt);
					} else
						cout << "NO" <<endl;
				}
			}
			break;
		case RUNNING:
			proc->AddTime(timeInPrevState);
			if (proc->GetLeftCPUBurst() == 0) {
				cTime = myRandom(proc->GetCPUBurst());
				cTime = (cTime > proc->GetRem()) ? proc->GetRem() : cTime;
				proc->SetCPUBurst(cTime);
			}
			else
				cTime = proc->GetLeftCPUBurst();

			if (cTime > sched->Get_Quantum())
				nEvt = new Event(proc, RUNNING, PREEMPT, CURRENT_TIME + sched->Get_Quantum());
			else if (cTime == proc->GetRem())
				nEvt = new Event(proc, RUNNING, DONE, CURRENT_TIME + proc->GetRem());
			else
				nEvt = new Event(proc, RUNNING, BLOCKED, CURRENT_TIME + cTime);

			orderedPush(&eQ, nEvt);
			CURRENT_RUNNING_PROCESS = proc;
			CALL_SCHEDULER = false;
			cout <<"READY -> RUNNG cb=" << proc->GetLeftCPUBurst() << " rem=" << proc->GetRem() << " prio=" << proc->GetDynPrio() << endl;
			break;
		case BLOCKED:
			proc->AddCPUTime(timeInPrevState);
			proc->SetCPUBurst(0);
			if (ioState == 0)
				ioStart_ts = CURRENT_TIME;
			ioState++;

			cTime = myRandom(proc->GetIOBurst());
			nEvt = new Event(proc, BLOCKED, READY, CURRENT_TIME + cTime);
			orderedPush(&eQ, nEvt);
			CURRENT_RUNNING_PROCESS = NULL;
			CALL_SCHEDULER = true;
			cout << "RUNNG -> BLOCK  ib=" << cTime << " rem=" << proc->GetRem() << endl;
			break;
		case PREEMPT:
			proc->AddCPUTime(timeInPrevState);
			proc->ReduceCPUBurst(timeInPrevState);
			nEvt = new Event(proc, PREEMPT, READY, CURRENT_TIME);
			orderedPush(&eQ, nEvt);
			CALL_SCHEDULER = true;
			CURRENT_RUNNING_PROCESS = NULL;
			cout << "RUNNG -> READY  cb=" << proc->GetLeftCPUBurst() << " rem=" << proc->GetRem() << " prio=" << proc->GetDynPrio() << endl;
			if (sched->Get_SType() == "PRIO" ||  sched->Get_SType() == "PREPRIO" )
				proc->Reduce_Prio();
			break;
		case DONE:
			proc->AddCPUTime(timeInPrevState);
			CALL_SCHEDULER = true;
			CURRENT_RUNNING_PROCESS = NULL;
			pSum[proc->GetNum()] = proc;
			cout << "Done" << endl;
			break;
		}
		delete evt;
		evt = NULL;

		if (CALL_SCHEDULER) {
			if ((!eQ.empty() && eQ.front()->GetTimeStamp() == CURRENT_TIME) || (!inputQ->empty() && inputQ->front()->GetTimeStamp() == CURRENT_TIME) ) {
				continue;
			}
			CALL_SCHEDULER = false;
			if (CURRENT_RUNNING_PROCESS == NULL ) {
				CURRENT_RUNNING_PROCESS = sched->get_next_process();
				if (CURRENT_RUNNING_PROCESS == NULL)
					continue;
				nEvt = new Event(CURRENT_RUNNING_PROCESS, READY, RUNNING, CURRENT_TIME);
				orderedPush(&eQ, nEvt);
			}
		}
	}

	sched->Print_SType();

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
		totalSum[2] += pSum[i]->GetTimeStamp() - pSum[i]->GetArrivalTime();
		totalSum[3] += pSum[i]->GetTimeStamp() - pSum[i]->GetArrivalTime() - pSum[i]->GetTotalIO() - pSum[i]->GetTotalCPU();
		delete pSum[i];
	}

	delete [] pSum;

	totalSum[1] = totalIO;
	totalSum[4] = inputSize;
	cout << "SUM: " << CURRENT_TIME ;

	for (int i = 0; i < 5; i++) {
		if (i == 2 || i == 3)
			cout << setprecision(2) << fixed << ' ' << totalSum[i] / inputSize;
		else if (i == 4)
			cout << setprecision(3) << fixed << ' ' << totalSum[i] / CURRENT_TIME * 100;
		else
			cout << setprecision(2) << fixed << ' ' << totalSum[i] / CURRENT_TIME * 100;
	}
	cout << endl;
}
