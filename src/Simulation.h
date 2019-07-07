/*
 * Simulation.h
 *
 *  Created on: Jul 5, 2019
 *      Author: sungw
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <iostream>
#include "Scheduler.h"

class Simulation {
private:
	int *randvals;
	int ofs;
public:
	Simulation(int * rand);
	void startSim(Scheduler * sched, queue <Process *> *inputQ);
	int myRandom (int burst);
};



#endif /* SIMULATION_H_ */
