#include <iostream>
#include <fstream>
#include <iomanip>
#include "simUnitsMap.h"

using namespace std;

int main()
 {
  simUnitsMap sMap = simUnitsMap("data\\simu.bin.nik");
  cout << sMap.getSIMU(-34.458391368,   83.541597348) << endl;
  cout << sMap.getSIMU(-179.9583332 + 594./12, -55.87501355 + 354./12) << endl;
  sMap.saveToFile();
  system("pause");
 }
