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
#include <cstring>
#include <cmath>

using namespace std;

class simUnitsMap {
private:
  typedef vector<int> intVector;
// Some constants:
  static const int NSIMU = 212707;          // Total number of simulation units;  
  static const int xRes = 4320;             // Longitude resolution 
  static const int yRes = 1674;             // Latitude resolution
  static const float xMin = -179.9583332;   // Minimum longitude value
  static const float yMin = -55.87501355;   // Minimum latitude value
// Data containers:
  int * simUMap;                            // Map of simulation units
  int * ptr;                                // ptr[i] indicates the number of points (cells) that belong to i-th simulation unit;
  vector<intVector> xPoints, yPoints;
  int round(float val);
public:
  simUnitsMap();
  simUnitsMap(string fileName);
  ~simUnitsMap();
  int getSIMU(double x, double y);          //
  int SIMU_per_cell(double x, double y);    //
  void saveToFile();
  void saveToFile_ESRIGrid();
};

// Default constructor
simUnitsMap::simUnitsMap()
 {
// Creating own coordinate system:
  ptr = new int[NSIMU+1];
  simUMap = new int[xRes * yRes];
  for (int j = 0; j < yRes; j++) {
    for (int i = 0; i < xRes; i++) {
      int tmp = int(j/6) * 720 + int(i/6);
      simUMap[j * xRes + i] = tmp;
      ptr[tmp+1]++;
    }
  }
  ptr[0] = 0;
  for (int i = 1; i <= NSIMU; i++) {
    ptr[i] = ptr[i-1] + ptr[i];
  }
 }

// Constructor
simUnitsMap::simUnitsMap(string fileName)
 {
  ptr = new int[NSIMU+1];
  simUMap = new int[xRes * yRes];
  for (int i = 0; i < xRes * yRes; i++) simUMap[i] = -1;
  ifstream f;
  f.open(fileName.c_str(), ios::in | ios::binary);

  if (f.is_open()) {
    f.read(reinterpret_cast<char *>(ptr), sizeof(int) * (NSIMU + 1));
    int NPTS = ptr[NSIMU];
//    for (int i = 0; i <= NSIMU; i++) ptr[i] = 0;
    int SIMU = 0;
//cout << "xRes * yRes = " << xRes * yRes << endl;
//system("pause");
    for (int i = 0; i < NPTS; i++) {
      if (i > ptr[SIMU]) SIMU++;
      int xx, yy;
      if (f.eof()) {
        cout << "End of file!" << endl;
        break;
      }
      f.read(reinterpret_cast<char *>(&xx), sizeof(int));
      if (f.eof()) {
        cout << "End of file!" << endl;
        break;
      }
      f.read(reinterpret_cast<char *>(&yy), sizeof(int));
//      int x = int(xx/6);
//      int y = int(yy/6);
//      int tmp = y * 720 + x;
//cout << "yy * xRes + xx = " << yy * xRes + xx << endl;
//      simUMap[yy * xRes + xx] = tmp;
      simUMap[yy * xRes + xx] = SIMU;
//      ptr[tmp+1]++;
// 212984
//if (i == 157205) {
//  cout << "> " << i << endl;
//  system("pause");
//}
    }
//cout << "> 2" << endl;
//system("pause");
    ptr[0] = 0;
    for (int i = 1; i <= NSIMU; i++) {
      ptr[i] = ptr[i-1] + ptr[i];
    }
    f.close();
    cout << "Successfully read from binary file: " << fileName << endl;
  } else {
    cout << "Unable to open file!" << endl;
  }
 }

// Destructor
simUnitsMap::~simUnitsMap()
 {
  delete []simUMap;
  delete []ptr;
 }

//================
// Class methods:
//================

int simUnitsMap::round(float val)
 {
  if ((val + 0.5) >= (int(val) + 1))
    return int(val)+1;
  else
    return int(val);
 }

int simUnitsMap::getSIMU(double x, double y)
 {
  int xID = round(12. * (x - xMin));
  int yID = yRes - 1 - round(12. * (y - yMin)) ;
  if ((xID < 0) || (xID >= xRes) || (yID < 0) || (yID >= yRes)) return -2;
  return simUMap[yID * xRes + xID];
 }

int simUnitsMap::SIMU_per_cell(double x, double y)
 {
  set<int> res;
  int xID = round(12. * (x - xMin));
  int yID = yRes - 1 - round(12. * (y - yMin)) ;
  if ((xID < 0) || (xID >= xRes) || (yID < 0) || (yID >= yRes)) ;
  else {
    for (int i = 5; i >= 0; i--) {
      if ((xID + i) < xRes) {
        for (int j = 5; j >= 0; j--) {
          if ((yID + j) < yRes) {
            if (simUMap[(yID + j) * xRes + (xID + i)] >= 0) {
              res.insert(simUMap[(yID + j) * xRes + (xID + i)]);
            }
          }
        }
      }
    }
  }
  int result = res.size();
  return result;
 }

void simUnitsMap::saveToFile()
 {
  xPoints.resize(NSIMU);
  yPoints.resize(NSIMU);
  for (int j = 0; j < yRes; j++) {
    for (int i = 0; i < xRes; i++) {
      int tmp = simUMap[j * xRes + i];
      if (tmp >=0) {
        xPoints[tmp].push_back(i);
        yPoints[tmp].push_back(j);
      }
    }
  }
  int vSize = 2 * ptr[NSIMU];
// temporary array for output of geographic indeces:
  int * v = new int[vSize];
  int id = 0;
  for (int i = 0; i < NSIMU; i++) {
    for (int j = 0; j < xPoints[i].size(); j++) {
      v[2 * id]     = xPoints[i][j];
      v[2 * id + 1] = yPoints[i][j];
      id++;
    }
  }
// Writing to file:
  ofstream f;
  string fileName = "simu.bin";
  f.open(fileName.c_str(), ios::out | ios::binary);
  if (f.is_open()) {
    f.write(reinterpret_cast<char *>(ptr), (NSIMU + 1) * sizeof(int));
    f.write(reinterpret_cast<char *>(v), vSize * sizeof(int));
    f.close();
    cout << "Successfully written to binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
  delete []v;
 }

void simUnitsMap::saveToFile_ESRIGrid()
 {
  ofstream f;
  string fileName = "SIMU.asc";
  f.open(fileName.c_str(),ios::out);
  if (f.is_open()) {
    f << "NCOLS " << xRes << endl;
    f << "NROWS " << yRes << endl;
    f << "XLLCORNER " << xMin << endl;
    f << "YLLCORNER " << yMin << endl;
    f << "CELLSIZE " << 0.5/6  << endl;
    f << "NODATA_VALUE -9999" << endl;
    for (int j = 0; j < yRes; j++) {
      for (int i = 0; i < xRes; i++) {
        int tmp = simUMap[j * xRes + i];
        f << tmp << " ";
      }
      f << endl;
    }
    f.close();     
    cout << "Successfully written to binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }

#endif
