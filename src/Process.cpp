/*
 * Process.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: sungw
 */

#include <iostream>
#include "Process.h"
using namespace std;

Process::Process (int input[]) {
	num = pNum++;
	arrival = input[0];
	tCPU = input[1];
	CPU_Burst = input[2];
	IO_Burst = input[3];
	state_ts = 0;
	totalCPU = 0;
	totalIO = 0;
	static_prio = input[5];
	cout << arrival << " " << tCPU << " " << CPU_Burst << " " << IO_Burst << endl;
}

int Process::TimeStamp() {
	return state_ts;
}

int Process::GetNum() {
	return num;
}

int Process::GetCPUBurst() {
	return CPU_Burst;
}

int Process::GetIOBurst() {
	return IO_Burst;

}
