/*
 * SchedulerMain.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: Sungwoo Chun
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "Process.h"
#include "Scheduler.h"
#include "F_Scheduler.h"
#include "L_Scheduler.h"
#include "S_Scheduler.h"
#include "P_Scheduler.h"
#include "Simulation.h"
using namespace std;

int Process::pNum = 0;

int main (int argc, char * argv[]) {
	int c;
	int quantum = 10000;	//default quantum
	int maxprio = 4;	//default maxprio
	char *cvalue;	//variable to store arguments follow -s
	bool vOpt = false; //variable to save -v option
	ifstream iFile;	//input file stream
	ifstream rFile;	//random file strea

	while ((c = getopt (argc, argv, "vtes:")) != -1) {
		switch (c) {
		case 'v':
			vOpt = true;
			break;
		case 't':
			break;
		case 'e':
			break;
		case 's':
			cvalue = optarg;
			if (cvalue[0] == 'F' || cvalue[0] == 'L' || cvalue[0] == 'S')
				continue;
			else if (cvalue[0] == 'R' || cvalue[0] == 'P' || cvalue[0] == 'E') {
				int i = 0;
				quantum = 0;
				while (isdigit(cvalue[++i])) //getting optional quantum value
					quantum = quantum * 10 + (cvalue[i]-'0');
 				if (cvalue[i] == ':') { //getting optional maxprio value
					maxprio = 0;
					while (isdigit(cvalue[++i]))
						maxprio = maxprio * 10 + (cvalue[i]-'0');
				}
			} else {
				cout << "Unknown Scheduler spec: -v {FLSRPE}" << endl;
				return 1;
			}
			break;
		case '?':
			fprintf (stderr, "Usage: ./sched [-v] inputfile randomfile\n");
			return 1;
		default:
			break;
		}
	}

	iFile.open(argv[optind]);
	if (!iFile.is_open()) {
		cout << "Not a valid inputfile " << argv[optind] << endl;
		return  1;
	}

	rFile.open(argv[optind+1]);
	if (!rFile.is_open()) {
		cout << "Not a valid inputfile " << argv[optind+1] << endl;
		return 1;
	}

	string readline;
	getline(rFile, readline);
	int rIndex = atoi(readline.c_str());
	int *rand = new int[rIndex];

	for (int i = 0; i < rIndex; i++) {
		getline(rFile, readline);
		rand[i] = atoi(readline.c_str());
	}

	Simulation * sim = new Simulation(rand);

	queue <Process *> pQ;
	int pch[5];

	//creating new process and pushing into the process queue
	while(getline(iFile, readline)) {
		char *pArg = new char[readline.length() + 1];
		strcpy(pArg, readline.c_str());
		pch[0] = atoi(strtok(pArg, " "));
		for (int i = 1; i < 4; i++) {
			pch[i] = atoi(strtok(NULL, " "));
		}
		pch[4] = sim->myRandom(maxprio);
		Process *p = new Process(pch);
		pQ.push(p);
		delete pArg;
	}

	Scheduler *scheduler;
	switch (cvalue[0]) {	//creating scheduler based on argument
	case 'F':
		scheduler = new F_Scheduler("FCFS", quantum);
		break;
	case 'L':
		scheduler = new L_Scheduler("LCFS", quantum);
		break;
	case 'S':
		scheduler = new S_Scheduler("SRTF", quantum);
		break;
	case 'R':
		scheduler = new F_Scheduler("RR", quantum);	//RR uses same scheduler as FCFS
		break;
	case 'P':
		scheduler = new P_Scheduler("PRIO", quantum);
		break;
	case 'E':
		scheduler = new P_Scheduler("PREPRIO", quantum);	//PREPRIO uses same scheduler as PRIO
		break;
	default:
		break;
	}

	sim->startSim(scheduler, &pQ, vOpt);	//starting simulation

	delete [] rand;
	delete sim;
	delete scheduler;
	return 0;
}
