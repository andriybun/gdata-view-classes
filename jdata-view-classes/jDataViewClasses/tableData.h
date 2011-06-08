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

using namespace std;

class tableData
{
private:
	typedef vector<string> str_vector_t;
	typedef vector<int> int_vector_t;
	map<long long, float> data;
	MDT descr;
	str_vector_t point;					// Point in multidimensional space, determining specific values for dimensions
	long long N;                           // Number of data records per simulation unit
public:
	tableData();
	tableData(string fileNameGdc);
	~tableData();
	// Inserts a value "val" corresponding to a vector of string coordinates "point" into the list.
	void insert(float val, str_vector_t point);
	// Inserts a value "val" corresponding to a vector of integer coordinates "point" into the list.
	void insert(float val, int_vector_t point);
	// ... inserts value for current point and paramName set in last position in point, then it clears last param
	void insert(float val, string paramName);
	// Updates a value "val" corresponding to a vector of coordinates "point" into the list.
	void update(float val, str_vector_t point);
	// Rename dataset:
	void rename(string name);
	// Rename dimensions of the dataset:
	void renameDims(str_vector_t vec);
	// Add new dimension:
	void addDim(string dimName, set<string> elements);
	void addDim(string dimName, set<int> elements);
	void addDim(string dimName, string element);
	void updateDimEl(string dimName, int posEl, string element);
	// Set values for point:
	void pointPush(string val);
	void pointPush(int val);
	void pointPop();
	void pointClear();
	// Clearing object
	void clear();
	// Append table by one of the dimensions
	int append(tableData & another, int dim);
	// Writing object to files *.GDT and *.GDC files
	bool SaveToFile(string outDir, string fileName);
};

#endif
