#include <iostream>
#include <cstdlib>
#include <fstream>

#include "simUnitsMap.h"

using namespace std;

int main()
 {
  simUnitsMap sMap = simUnitsMap("data\\simu.bin.nik");
cout << "SIMU corresponding to coordinates:" << endl;
  cout << sMap.getSIMU(-34.458391368, 83.541597348) << endl;
  cout << sMap.getSIMU(-33.791724968, 83.541597348) << endl;
  cout << sMap.getSIMU(-179.9583332 + 594./12, -55.87501355 + 354./12) << endl;
cout << "SUMU's per cell 0.5x0.5:" << endl;
  cout << sMap.SIMU_per_cell(-179.9583332 + 594./12, -55.87501355 + 354./12) << endl;
sMap.saveToFile_ESRIGrid();
/*  cout << "SIMU statistics:" << endl;
  int xRes = 4320;             // Longitude resolution 
  int yRes = 1674;             // Latitude resolution
  float xMin = -179.9583332;   // Minimum longitude value
  float yMin = -55.87501355;   // Minimum latitude value
  int max = 0;
  int sum = 0;
  int min = 999;
  int quantity = 0;
  for (int i = 0; i < xRes; i++) {
    for (int j = 0; j < yRes; j++) {
      int tmp = sMap.SIMU_per_cell(xMin + 0.5 * i, yMin + 0.5 * j);
      if (tmp) {
        if (tmp < min) min = tmp;
        if (tmp > max) max = tmp;
        sum += tmp;
        quantity++;
      }
    }  
  }
  cout << "min = " << min << endl;
  cout << "max = " << max << endl;
  cout << "avg = " << 1. * sum / quantity << endl;
*/
/*double v = 75;
double h = 275+1;
sMap.SIMU_per_cell(-179.9583332+h, -55.87501355+v+1.5);
cout << endl;
sMap.SIMU_per_cell(-179.9583332+h, -55.87501355+v+1);
cout << endl;
sMap.SIMU_per_cell(-179.9583332+h, -55.87501355+v+0.5);
cout << endl;
*/
//  sMap.saveToFile();
  system("pause");
 }

// 7 231 680
// 5 664 991
