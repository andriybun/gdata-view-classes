#include "activeSimUnits.h"

activeSimUnits::activeSimUnits()
 {

 }

activeSimUnits::~activeSimUnits()
 {

 }

//================
// Class methods:
//================

void activeSimUnits::insert(int val)
 {
  ASU.insert(val);
 }

void activeSimUnits::clear()
 {
  ASU.clear();
 }

void activeSimUnits::saveToFile(string fileName)
 {
  ofstream f;
  fileName += ".msu";
  f.open(fileName.c_str(), ios::out | ios::binary);
  if (f.is_open()) {
    int numSimU = ASU.size();
//    cout << "Number of active SU's: " << numSimU << endl;
    INV_BYTE_ORDER(numSimU);
    f.write(reinterpret_cast<char *>(&numSimU), sizeof(int));
    set<int>::iterator it;
//    cout << "Elements: " << endl;
    for ( it = ASU.begin() ; it != ASU.end(); it++ ) {
      int tmp = *it;
//      cout << tmp << endl;
      INV_BYTE_ORDER(tmp);
      f.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }
    f.close();
    cout << "Successfully written to binary file: " << fileName << endl;
  } else {
    cout << "Unable to save to file!" << endl;
  }
 }
