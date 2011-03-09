//  Name:			simUnitsData class
//  Author:			Andriy Bun
//  Date:			05.11.09
//  Description:	Class for storing and producing data for viewing with GDataView
//					Software. Is a friend class for MDT class.
//  Modified:		11.01.10 (Andriy Bun)
//					22.02.11 (Andriy Bun): added overloaded method to insert values for simulationn units corresponding to 
//							 coordinates in simUnitsMap

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
#include "simUnitsMap.h"

using namespace std;

enum distribute_value_t 
{
	DISTRIBUTE_PROPORTIONALLY,
	IS_CONSTANT
};

class simUnitsData
{
private:
	typedef vector<string> str_vector_t;
	typedef vector<float> float_vector_t;
	map<int, float_vector_t> data;
	simUnitsMap sMap;
	MDT descr;
	str_vector_t point;					// Point in multidimensional space, determining specific values for dimensions
	int N;								// Number of data records per simulation unit
public:
	simUnitsData();
	simUnitsData(string fileNameSimuBin);
	simUnitsData(string fileNameSimuBin, string fileNameMdt);
	~simUnitsData();
	// Inserts a value "val" corresponding to an active simulation unit SIMU and
	// vector of coordinates "point" into the list.
	void insert(int SIMU, float val);
	// ... inserts value for simulation units corresponding to coordinates (x, y)
	// and 
	void insert(double x, double y, float val,
				distribute_value_t distribute_value = DISTRIBUTE_PROPORTIONALLY);
	// ... inserts value for simulation units corresponding to coordinates (x, y)
	// and parameter name paramName set in last position in point, then it clears last param
	void insert(double x, double y, float val, string paramName, 
				distribute_value_t distribute_value);
	// Set simUnitsMap:
	void setMap(string fileNameSimuBin);
	// Rename dataset:
	void rename(string name);
	// Rename dimensions of the dataset:
	void renameDims(str_vector_t vec);
	// Add new dimension:
	void addDim(string dimName, set<string> elements);
	void addDim(string dimName, set<int> elements);
	void addDim(string dimName, string element);
	// Set values for point:
	void pointPush(string val);
	void pointPush(int val);
	void pointPop();
	void pointClear();
	// Clearing dataset:
	void clear();
	// Writing dataset to files *.MSU and *.MDC files
	bool SaveToFile(string outDir, string fileName);
};

#endif
