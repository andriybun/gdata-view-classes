//  Name:         tableData class
//  Author:       Andriy Bun
//  Date:         13.11.09
//  Modified:     11.01.10 (Andriy Bun)
//  Description:  Class for storing and producing data for viewing with 
//                GDagaView Software. Is a friend class for MDT class. 

#ifndef TABLEDATA_H_
#define TABLEDATA_H_

#include <map>
#include <vector>
#include <fstream>
#include <cstring>

#include "IntToStr.h"
#include "MDT.h"

#ifndef PATHSEPARATOR_
#define PATHSEPARATOR_

#ifdef unix
string pathSeparator = "/";
#else
string pathSeparator = "\\";
#endif

#endif


using namespace std;

class tableData {
private:
  typedef vector<string> strVector;
  map<long long, float> data;
  MDT descr;
  long long N;                           // Number of data records per simulation unit
  void INV_BYTE_ORDER64(long long &v);
  void INV_BYTE_ORDER_F(float &v);
  void INV_BYTE_ORDER(int &v);
public:
  tableData();
  tableData(string fileName_GDT);
  ~tableData();
  // Inserts a value "val" corresponding to a vector of coordinates "point" 
  // into the list. 
  void insert(float val, strVector point);
  // Updates a value "val" corresponding to a vector of coordinates "point" 
  // into the list. 
  void update(float val, strVector point);
  // Rename dataset:
  void rename(string name);
  // Rename dimensions of the dataset:
  void renameDims(strVector vec);
  // Add new dimension:
  void addDim(string dimName, set<string> elements);
  void addDim(string dimName, set<int> elements);
  void addDim(string dimName, string element);
  // Clearing object
  void clear();
  // Writing object to files *.GDT and *.GDC files
  bool SaveToFile(string outDir, string fileName);
};

// default constructor
tableData::tableData()
 {
  descr = MDT();
  N = descr.getN();
 }

// constructor
tableData::tableData(string fileName_GDT)
 {
  descr = MDT(fileName_GDT);
  N = descr.getN();
 }

// destructor
tableData::~tableData()
 {
/*  map<int, floatVector>::iterator it = data.begin();
  while (it != data.end()) {
    delete it->second;
    it++;
  }*/
 }

//================
// Class methods:
//================

void tableData::INV_BYTE_ORDER64(long long &v)
 {
  v = (v >> 56) |	                           // Move first byte to the end,
      ((v >> 40) & 0x00FF000000000000LLU) |    // move 2nd byte to 7th,
      ((v >> 24) & 0x0000FF0000000000LLU) |    // move 3rd byte to 6th,
      ((v >>  8) & 0x000000FF00000000LLU) |    // move 4th byte to 5th,
      ((v <<  8) & 0x00000000FF000000LLU) |    // move 5th byte to 4th,
      ((v << 24) & 0x0000000000FF0000LLU) |    // move 6th byte to 3rd,
      ((v << 40) & 0x000000000000FF00LLU) |    // move 7th byte to 2nd,
      (v << 56);                               // move last byte to start.
 }

void tableData::INV_BYTE_ORDER_F(float &v)
 {
  int tmp;
  memcpy(&tmp, &v, 4);
  tmp = (tmp >> 24) |	               // Move first byte to the end,
        ((tmp << 8) & 0x00FF0000) |    // move 2nd byte to 3rd,
        ((tmp >> 8) & 0x0000FF00) |    // move 3rd byte to 2nd,
        (tmp << 24);                   // move last byte to start.
  memcpy(&v, &tmp, 4);
 }

void tableData::INV_BYTE_ORDER(int &v)
 {
  v = (v >> 24) |	               // Move first byte to the end,
      ((v << 8) & 0x00FF0000) |    // move 2nd byte to 3rd,
      ((v >> 8) & 0x0000FF00) |    // move 3rd byte to 2nd,
      (v << 24);                   // move last byte to start.
 }

void tableData::insert(float val, strVector point)
 {
  if (val != 0)
    data.insert(make_pair(descr.getHash(point), val));
 }

void tableData::update(float val, strVector point)
 {
  if (val != 0) {
    long long card = descr.getHash(point);
    data.erase(card);
    data.insert(make_pair(card, val));
  }
 }

void tableData::rename(string name)
 {
  descr.paramName = name;
 }

void tableData::renameDims(strVector vec)
 {
  descr.dimNames = vec;
 }

void tableData::addDim(string dimName, set<string> elements)
 {
  strVector tmp;
  set<string>::iterator it = elements.begin();
  while(it != elements.end()) {
    tmp.push_back(*it);
    it++;
  }
  descr.addDim(dimName,tmp);
 }

void tableData::addDim(string dimName, set<int> elements)
 {
  strVector tmp;
  set<int>::iterator it = elements.begin();
  while(it != elements.end()) {
    tmp.push_back(IntToStr(*it));
    it++;
  }
  descr.addDim(dimName,tmp);
 }

void tableData::addDim(string dimName, string element)
 {
  strVector tmp;
  tmp.push_back(element);
  descr.addDim(dimName,tmp);
 }

void tableData::clear()
 {
  data.clear();
  descr.clear();
 }

bool tableData::SaveToFile(string outDir, string fileName)
 {
//****************************
//**** Writimg *.GDC file ****
//****************************
  if (data.size() == 0) {
    cout << "ERROR! Attempt to write empty data." << endl;
    return false;
  }
  fileName = outDir + pathSeparator + fileName;
  string fileNameTmp = fileName + ".gdc";
  ofstream f;
  f.open(fileNameTmp.c_str(), ios::out | ios::binary);
  if (f.is_open()) {
    // Writing info about dimensions:
    int tmp_int = descr.getNDims();
    INV_BYTE_ORDER(tmp_int);
    f.write(reinterpret_cast<char *>(&tmp_int), sizeof(int));
    for (int i = 0; i < descr.nDims; i++) {
      tmp_int = descr.dimCardinals[i];
      INV_BYTE_ORDER(tmp_int);
      f.write(reinterpret_cast<char *>(&tmp_int), sizeof(int));
    }
    // Writing hash data:
    tmp_int = data.size();
    int numVal = tmp_int;
    long long * vl = new long long [numVal];
    float * vf = new float [numVal];
    INV_BYTE_ORDER(tmp_int);
    f.write(reinterpret_cast<char *>(&tmp_int), sizeof(int));
    int i = 0;
    map<long long, float>::iterator it = data.begin();
    while (it != data.end()) {
      long long tmp_long = it->first;
      INV_BYTE_ORDER64(tmp_long);
      vl[i] = tmp_long;
      float tmp_float = it->second;
      INV_BYTE_ORDER_F(tmp_float);
      vf[i] = tmp_float;
      i++;
      it++;
    }
    f.write(reinterpret_cast<char *>(vl), sizeof(long long) * numVal);
    f.write(reinterpret_cast<char *>(vf), sizeof(float) * numVal);
    delete []vl;
    delete []vf;
    f.close();
    cout << "Successfully written to binary file: " << fileNameTmp << endl;
  } else {
    cout << "Unable to save to file!" << endl;
    return false;
  }
//****************************
//**** Writimg *.GDT file ****
//****************************
  descr.SaveToFile(fileName + ".gdt", "TABLE");
  return true;
 }

#endif
