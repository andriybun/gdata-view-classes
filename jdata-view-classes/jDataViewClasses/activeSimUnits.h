//  Name:         activeSimUnits class
//  Author:       Andriy Bun
//  Date:         05.11.09
//  Description:  Class for creating list of active simulation units in the
//                GLOBIOM GUI format

#ifndef ACTIVESIMUNITS_H_
#define ACTIVESIMUNITS_H_

#include <set>
#include <iostream>
#include <fstream>
#include <string>

#include "endianness.h"

using namespace std;

class activeSimUnits {
private:
  set<int> ASU;             // List of active simulation units;
public:
  activeSimUnits();
  ~activeSimUnits();
  void insert(int val);     // Inserts active simulation unit # into the list
  void clear();
  void saveToFile(string fileName);

};

#endif
