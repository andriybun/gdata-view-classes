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
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include <assert.h>

#include "common.h"
#include "endianness.h"
#include "MDT.h"
#include "baseData.h"
#include "simUnitsMap.h"

enum distribute_value_t 
{
	DISTRIBUTE_PROPORTIONALLY,
	IS_CONSTANT
};

template < typename structT > class structWriter;

class simUnitsData : public baseData
{
private:
	template < typename structT > friend class structWriter;
	std::map<int, float_vector_t> data;
	simUnitsMap sMap;
public:
	simUnitsData();
	simUnitsData(std::string fileNameSimuBin);
	simUnitsData(std::string fileNameSimuBin, std::string fileNameMdt);
	~simUnitsData();
	// Set simUnitsMap:
	void setMap(std::string fileNameSimuBin);
	// Inserts a value "val" corresponding to an active simulation unit SIMU and
	// vector of coordinates "point" into the list.
	bool insert(int SIMU, float val);
	// ... inserts value for simulation units corresponding to coordinates (x, y)
	// and 
	bool insert(double x, double y, float val, distribute_value_t distribute_value = DISTRIBUTE_PROPORTIONALLY);
	// ... inserts value for simulation units corresponding to coordinates (x, y)
	// and parameter name paramName set in last position in point, then it clears last param
	bool insert(double x, double y, float val, std::string paramName, distribute_value_t distribute_value = DISTRIBUTE_PROPORTIONALLY);
	// ... inserts value for simulation unit SIMU, for parameter corresponding to element at 
	// position coord in last dimension of descr
	bool insertByLastCoord(int SIMU, int coord, float val);
	// ... inserts value for simulation units at position (x, y), for parameter corresponding to 
	// element at position coord in last dimension of descr
	bool insertByLastCoord(double x, double y, int coord, float val, distribute_value_t distribute_value = DISTRIBUTE_PROPORTIONALLY);
	// Get address of description:
	simUnitsMap & getSimUnitsMap();
	// Clearing dataset:
	void clear();
	// Writing dataset to files *.MSU and *.MDC files
	bool SaveToFile(std::string outDir, std::string fileName);
};

#endif
