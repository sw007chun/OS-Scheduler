/*
 * SchedulerMain.cpp
 *
 *  Created on: Jul 3, 2019
 *      Author: sungw
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cctype>
#include <cstring>
using namespace std;

int main (int argc, char * argv[]) {
	int c;
	int index;
	int quantum = 10000;
	int maxprio = 4;
	char *p;
	char *cvalue;
	char sType;
	ifstream iFile;
	ifstream rFile;

	while ((c = getopt (argc, argv, "vtes:")) != -1) {
		switch (c) {
		case 'v':
			break;
		case 't':
			break;
		case 'e':
			break;
		case 's':
			cvalue = optarg;
			if (cvalue[0] == 'F' || cvalue[0] == 'L' || cvalue[0] == 'S')
				sType = cvalue[0];
			else if (cvalue[0] == 'R' || cvalue[0] == 'P' || cvalue[0] == 'E') {
				sType = cvalue[0];
				int i = 0;
				quantum = 0;
				while (isdigit( cvalue[++i]) )
						quantum = quantum * 10 + (cvalue[i]-'0');
 				if (cvalue[i] == ':') {
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
			fprintf (stderr, "Usage: ./sched [-v] inputfile randomfile");
			return 1;
		default:
			break;
		}
		cout << "c: " << c << ", Type: " << sType << ", Q: " << quantum << ", M: " << maxprio << endl;
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

	return 0;
}
