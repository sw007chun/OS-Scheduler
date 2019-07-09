/*
 * S_Scheduler.h
 *
 *  Created on: Jul 8, 2019
 *      Author: sungw
 */

#ifndef S_SCHEDULER_H_
#define S_SCHEDULER_H_

#include "Scheduler.h"

class S_Scheduler : public Scheduler {
private:
	 list <Process *> runQ;
public:
	S_Scheduler(string type, int q);
	~S_Scheduler() {};
	void add_to_queue(Process *p);
	Process* get_next_process();
};



#endif /* S_SCHEDULER_H_ */
