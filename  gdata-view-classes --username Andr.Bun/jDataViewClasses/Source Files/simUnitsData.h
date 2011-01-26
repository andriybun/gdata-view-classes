//  Name:         simUnitsData class
//  Author:       Andriy Bun
//  Date:         05.11.09
//  Modified:     11.01.10 (Andriy Bun)
//  Description:  Class for storing and producing data for viewing with GDataView
//                Software. Is a friend class for MDT class.

#ifndef SIMUNITSDATA_H_
#define SIMUNITSDATA_H_

#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "common.h"
#include "endianness.h"
#include "MDT.h"

using namespace std;

class simUnitsData
{
private:
	typedef vector<string> strVector;
	typedef vector<float> floatVector;
	map<int, floatVector> data;
	MDT descr;
	int N;                           // Number of data records per simulation unit
public:
	simUnitsData();
	simUnitsData(string fileName_MDT);
	~simUnitsData();
	// Inserts a value "val" corresponding to an active simulation unit SIMU and
	// vector of coordinates "point" into the list.
	void insert(int SIMU, float val, strVector point);
	// Rename dataset:
	void rename(string name);
	// Rename dimensions of the dataset:
	void renameDims(strVector vec);
	// Add new dimension:
	void addDim(string dimName, set<string> elements);
	void addDim(string dimName, set<int> elements);
	void addDim(string dimName, string element);
	// Clearing dataset:
	void clear();
	// Writing dataset to files *.MSU and *.MDC files
	bool SaveToFile(string outDir, string fileName);
};

#endif
