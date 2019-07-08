/*
 * L_Scheduler.h
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */

#ifndef L_SCHEDULER_H_
#define L_SCHEDULER_H_

#include "Scheduler.h"

class L_Scheduler : public Scheduler {
private:
	 stack <Process *> runQ;
public:
	L_Scheduler(int q);
	~L_Scheduler() {};
	void add_to_queue(Process *p);
	Process* get_next_process();
};




#endif /* L_SCHEDULER_H_ */
