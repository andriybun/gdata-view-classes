#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include "activeSimUnits.h"

using namespace std;

int main()
 {
  activeSimUnits obj;
  obj.insert(1);
  obj.insert(7);
  obj.insert(3);
  obj.insert(5);
  obj.insert(2);
  obj.insert(1);
  obj.saveToFile("myRegion");
  system("pause");
 }
