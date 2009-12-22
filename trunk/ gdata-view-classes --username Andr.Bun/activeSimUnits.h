//  Name:         activeSimUnits class
//  Author:       Andriy Bun
//  Date:         05.11.09
//  Description:  Class for creating list of active simulation units in the
//                GLOBIOM GUI format

#ifndef ACTIVESIMUNITS_H_
#define ACTIVESIMUNITS_H_

#include <set>
#include <fstream>
#include <cstring>

using namespace std;

class activeSimUnits {
private:
  set<int> ASU;             // List of active simulation units;
  void INV_BYTE_ORDER(int &v);
public:
  activeSimUnits();
  ~activeSimUnits();
  void insert(int val);     // Inserts active simulation unit # into the list
  void clear();
  void saveToFile(string fileName);

};

activeSimUnits::activeSimUnits()
 {

 }

activeSimUnits::~activeSimUnits()
 {

 }

//================
// Class methods:
//================

void activeSimUnits::INV_BYTE_ORDER(int &v)
 {
  v = (v >> 24) |	               // Move first byte to the end,
      ((v << 8) & 0x00FF0000) |    // move 2nd byte to 3rd,
      ((v >> 8) & 0x0000FF00) |    // move 3rd byte to 2nd,
      (v << 24);                   // move last byte to start.
 }

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

#endif
