/*
 * Scheduler.cpp
 *
 *  Created on: Jul 6, 2019
 *      Author: sungw
 */

#include "Scheduler.h"


Scheduler::Scheduler (queue <Process *> *pQ) : allP(pQ) {

};
void Scheduler::push(Process *p) {
	runQ.push(p);
}
Process* Scheduler::pop() {
	Process * p = allP->front();
	allP->pop();
	return p;
}

//Scheduler::~Scheduler() {
//	delete runQ;
//}
