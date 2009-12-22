#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include "simUnitsData.h"

using namespace std;

int main()
 {
  simUnitsData obj;
  vector<string> point;
  point.push_back("- 0");
  string b;
  b = IntToStr(2001);
  point.push_back(b);
  point.push_back("Land area");
  
  for (int i = 0; i < 6; i++) {
    obj.insert(i, (i+1)*(i+1), point);
  }
  point.clear();
  point.push_back("- 1");
  point.push_back("2010");
  point.push_back("Land area");
  for (int i = 0; i < 6; i++) {
    obj.insert(i, (i+2)*(i+1), point);
  }
  obj.SaveToFile("-MYREG");
  obj.clear();
  obj.SaveToFile("myRegion");
  system("pause");
 }
