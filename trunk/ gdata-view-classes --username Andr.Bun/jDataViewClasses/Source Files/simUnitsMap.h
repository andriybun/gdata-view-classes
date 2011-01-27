//  Name:           simUintsMap class
//  Author:         Andriy Bun
//  Date:           04.11.09
//  Description:    class for (pre-)processing binary maps of simulation units in 
//                  GLOBIOM GUI format

#ifndef SIMUNITSMAP_H_
#define SIMUNITSMAP_H_

#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include <cmath>

#define RESOLUTION_RATIO 6			// proportion of resolutions (bigger cells to smaller)

using namespace std;

class simUnitsMap
{
private:
	typedef vector<int> intVector;
	// Some constants:
	static const int NSIMU;       // Total number of simulation units;  
	static const int xRes;        // Longitude resolution 
	static const int yRes;        // Latitude resolution
	static const float xMin;		// Minimum longitude value
	static const float yMin;		// Minimum latitude value
	// Data containers:
	int * simUMap;                            // Map of simulation units
	int * ptr;                                // ptr[i] indicates the number of points (cells) that belong to i-th simulation unit;
	vector<intVector> xPoints, yPoints;
	int round(float val);
public:
	simUnitsMap();
	simUnitsMap(string fileName);
	simUnitsMap(const simUnitsMap & sMap);
	simUnitsMap& operator=(const simUnitsMap & sMap);
	~simUnitsMap();
	int getSIMU(double x, double y);          //
	int SIMU_per_cell(double x, double y);    //
	void saveToFile();
	void saveToFile_ESRIGrid();
};

#endif
