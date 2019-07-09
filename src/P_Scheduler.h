/*
 * P_Scheduler.h
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */

#ifndef P_SCHEDULER_H_
#define P_SCHEDULER_H_

#include "Scheduler.h"

class P_Scheduler : public Scheduler {
private:
	 list <Process *> *activeQ;
	 list <Process *> *expiredQ;
public:
	P_Scheduler(string type, int q);
	~P_Scheduler();
	void add_to_queue(Process *p);
	Process* get_next_process();
};




#endif /* P_SCHEDULER_H_ */
