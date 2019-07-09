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
//random value function
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
//pushing new event in chronological and arriving order
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
//removing future event of process p in case of preemption from preprio scheduler
void removeFuture(list <Event *> *eQ, Process *p ) {
	list <Event *>::iterator it;
	for (it = eQ->begin() ; it != eQ->end(); it++) {
		if ( (*it)->evtProcess()->GetNum() == p->GetNum() ) {
			eQ->erase(it);
			return;
		}
	}
}
//getting the end time of current process in the event queue
int getFuture(list <Event *> *eQ, Process *p ) {
	list <Event *>::iterator it;
	for (it = eQ->begin() ; it != eQ->end(); it++) {
		if ( (*it)->evtProcess()->GetNum() == p->GetNum() ) {
			return (*it)->GetTimeStamp();
		}
	}
	return -1;
}

void Simulation::startSim (Scheduler * sched, queue <Process *> *inputQ, bool vOpt) {
	int inputSize = inputQ->size();	//number of input processes
	Process **pSum = new Process*[inputSize]; //this array is for summary of process execution at the end of simulation
	double totalSum[5] = {0.0};	//for future summary information
	bool CALL_SCHEDULER = false;
	int ioState = 0, ioStart_ts = 0, totalIO = 0; //counters to check if least one IO running time and get the IO runtime
	int CURRENT_TIME = 0;
	int timeInPrevState = 0;
	int cTime; //CPU Burst
	list <Event *> eQ; //event queue
	Event *evt; // current event
	Event *nEvt; // newly added Event
	Process *proc;	//event process
	Process *CURRENT_RUNNING_PROCESS = NULL;

	//pushing event creation event from input process queue
	while (!inputQ->empty()) {
		eQ.push_back(new Event(inputQ->front(), CREATED, READY, inputQ->front()->GetTimeStamp()));
		inputQ->pop();
	}

	while (!eQ.empty()) {
		evt = eQ.front();
		eQ.pop_front();
		proc = evt->evtProcess();
		CURRENT_TIME = evt->GetTimeStamp();
		timeInPrevState = CURRENT_TIME - proc->GetTimeStamp();

		//my simulation go from Running -> Preemption -> Ready, so no need to print in case event comes from Preempt state
		if (vOpt && evt->GetPrevState() != PREEMPT)
			cout << CURRENT_TIME << ' ' << proc->GetNum() << ' ' << timeInPrevState << ": " ;

  		switch(evt->Transition()) {
		case READY:
			if (evt->GetPrevState() == BLOCKED) {
				proc->Reset_Prio();	//reset dynamic priority
				proc->AddIOTime(timeInPrevState);	//add process' IO running time
				ioState--;
				if (ioState == 0)	//if not a single IO is running count its running time till now
					totalIO += CURRENT_TIME - ioStart_ts;
			}

			CALL_SCHEDULER = true;
			sched->add_to_queue(proc);	//add to the run queue

			if (evt->GetPrevState() != PREEMPT) {
				if (vOpt)
					cout << GetStateName(evt->GetPrevState()) << " -> READY " << endl;
				if (sched->Get_SType() == "PREPRIO" && CURRENT_RUNNING_PROCESS != NULL) {
					int compPrio = proc->GetDynPrio() > CURRENT_RUNNING_PROCESS->GetDynPrio();	//if newly ready process has higher dynamic priority than current running process
					int compTime = getFuture(&eQ, CURRENT_RUNNING_PROCESS) > CURRENT_TIME;	//if current running process will end now or not
					if (vOpt) {
						cout << "---> PRIO preemption "<< CURRENT_RUNNING_PROCESS->GetNum() << " by " << proc->GetNum() << " ? " << compPrio;
						cout << " TS=" << getFuture(&eQ, CURRENT_RUNNING_PROCESS) << " now=" << CURRENT_TIME << ") --> ";
					}
					if (compPrio && compTime) {	//if preemption condition is satisfied
						if (vOpt)
							cout << "YES" << endl;
						removeFuture(&eQ, CURRENT_RUNNING_PROCESS);	//remove future event of current process
						orderedPush(&eQ, new Event(CURRENT_RUNNING_PROCESS, RUNNING, PREEMPT, CURRENT_TIME));	//current process is preempted
					} else
						if (vOpt)
							cout << "NO" <<endl;
				}
			}
			break;
		case RUNNING:
			proc->AddTime(timeInPrevState);	//process time progressing
			if (proc->GetLeftCPUBurst() == 0) {	//if not more CPU burst is left
				cTime = myRandom(proc->GetCPUBurst());	//get new random CPU burst
				cTime = (cTime > proc->GetRem()) ? proc->GetRem() : cTime;	//if new CPU Burst is longer than remaining CPU time change it
				proc->SetCPUBurst(cTime);
			}
			else
				cTime = proc->GetLeftCPUBurst();

			if (cTime > sched->Get_Quantum())	//if cpu burst is longer than the quantum get preempted
				nEvt = new Event(proc, RUNNING, PREEMPT, CURRENT_TIME + sched->Get_Quantum());
			else if (cTime == proc->GetRem())	//if cpu burst is same as remaining time process is done
				nEvt = new Event(proc, RUNNING, DONE, CURRENT_TIME + proc->GetRem());
			else	//else it gets blocked
				nEvt = new Event(proc, RUNNING, BLOCKED, CURRENT_TIME + cTime);

			orderedPush(&eQ, nEvt);
			CURRENT_RUNNING_PROCESS = proc;
			CALL_SCHEDULER = false;
			if (vOpt)
				cout <<"READY -> RUNNG cb=" << proc->GetLeftCPUBurst() << " rem=" << proc->GetRem() << " prio=" << proc->GetDynPrio() << endl;
			break;
		case BLOCKED:
			proc->AddCPUTime(timeInPrevState);
			proc->SetCPUBurst(0);
			if (ioState == 0)	//if there was no io process running
				ioStart_ts = CURRENT_TIME;	//start at least one io running time
			ioState++;

			cTime = myRandom(proc->GetIOBurst());
			orderedPush(&eQ, new Event(proc, BLOCKED, READY, CURRENT_TIME + cTime));
			CURRENT_RUNNING_PROCESS = NULL;
			CALL_SCHEDULER = true;
			if (vOpt)
				cout << "RUNNG -> BLOCK  ib=" << cTime << " rem=" << proc->GetRem() << endl;
			break;
		case PREEMPT:
			proc->AddCPUTime(timeInPrevState);
			proc->ReduceCPUBurst(timeInPrevState);
			orderedPush(&eQ, new Event(proc, PREEMPT, READY, CURRENT_TIME));	//all preempted process will be ready
			CALL_SCHEDULER = true;
			CURRENT_RUNNING_PROCESS = NULL;
			if (vOpt)
				cout << "RUNNG -> READY  cb=" << proc->GetLeftCPUBurst() << " rem=" << proc->GetRem() << " prio=" << proc->GetDynPrio() << endl;
			if (sched->Get_SType() == "PRIO" ||  sched->Get_SType() == "PREPRIO" )
				proc->Reduce_Prio();	//reduce dynamic priority for two scheduleres
			break;
		case DONE:
			proc->AddCPUTime(timeInPrevState);
			CALL_SCHEDULER = true;
			CURRENT_RUNNING_PROCESS = NULL;
			pSum[proc->GetNum()] = proc; //this is to get process order for later print out
			if (vOpt)
				cout << "Done" << endl;
			break;
		}
		delete evt;
		evt = NULL;

		if (CALL_SCHEDULER) {
			if (!eQ.empty() && eQ.front()->GetTimeStamp() == CURRENT_TIME)	//if there is an event left at current time, it takes precedence over scheduling
				continue;

			CALL_SCHEDULER = false;
			if (CURRENT_RUNNING_PROCESS == NULL) {
				CURRENT_RUNNING_PROCESS = sched->get_next_process();
				if (CURRENT_RUNNING_PROCESS == NULL)
					continue;
				orderedPush(&eQ, new Event(CURRENT_RUNNING_PROCESS, READY, RUNNING, CURRENT_TIME));
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
	totalSum[1] = totalIO;
	totalSum[4] = inputSize;

	delete [] pSum;

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
