//  Name:         tableData class
//  Author:       Andriy Bun
//  Date:         13.11.09
//  Modified:     11.01.10 (Andriy Bun)
//  Description:  Class for storing and producing data for viewing with
//                GDagaView Software. Is a friend class for MDT class.

#ifndef TABLEDATA_H_
#define TABLEDATA_H_

#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

#include "common.h"
#include "endianness.h"
#include "IntToStr.h"
#include "MDT.h"
#include "baseData.h"

class tableData : public baseData
{
private:
	std::map<long long, float> data;
public:
	tableData();
	tableData(std::string fileNameGdc);
	~tableData();
	// Inserts a value "val" corresponding to a vector of string coordinates "point" into the list.
	void insert(float val, str_vector_t point);
	// Inserts a value "val" corresponding to a vector of integer coordinates "point" into the list.
	void insert(float val, int_vector_t point);
	// ... inserts value for current point and paramName set in last position in point, then it clears last param
	void insert(float val, std::string paramName);
	// Updates a value "val" corresponding to a vector of coordinates "point" into the list.
	void update(float val, str_vector_t point);
	void updateDimEl(std::string dimName, int posEl, std::string element);
	// Clearing object
	void clear();
	// Append table by one of the dimensions
	int append(tableData & another, int dim);
	// Writing object to files *.GDT and *.GDC files
	bool SaveToFile(std::string outDir, std::string fileName);
};

#endif
