/*
 * Scheduler.cpp
 *
 *  Created on: Jul 6, 2019
 *      Author: sungw
 */

#include "Scheduler.h"


Scheduler::Scheduler (string st) : sType(st) {

}
string Scheduler::Get_SType() {
	return sType;
}

//Process* Scheduler::CurrentProcess() {
//	if (!runQ.empty()) {
//		cout << "Current: " << runQ.size() << ' ' << runQ.front()->GetNum() << endl;
//		return runQ.front();
//	}
//	else
//		return NULL;
//}
//Process* Scheduler::Get_New_Process() {
//	Process * p = allP->front();
//	allP->pop();
//	return p;
//}

//Scheduler::~Scheduler() {
//	delete runQ;
//}
