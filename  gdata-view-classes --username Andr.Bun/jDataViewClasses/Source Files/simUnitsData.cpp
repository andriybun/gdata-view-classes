#include "simUnitsData.h"

// default constructor
simUnitsData::simUnitsData()
 {
  descr = MDT();
  N = descr.getN();
 }

// constructor
simUnitsData::simUnitsData(string fileName_MDT)
 {
  descr = MDT(fileName_MDT);
  N = descr.getN();
 }

// destructor
simUnitsData::~simUnitsData()
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

void simUnitsData::insert(int SIMU, float val, strVector point)
 {
  if (data.find(SIMU) == data.end()) {
    floatVector tmp;// = new floatVector;
    data.insert(make_pair(SIMU, tmp));
  }
  int card = descr.getHash(point);
  if (data[SIMU].size() < (card + 1)) data[SIMU].resize(card + 1);
  data[SIMU][card] = val;
 }

void simUnitsData::rename(string name)
 {
  descr.paramName = name;
 }

void simUnitsData::renameDims(strVector vec)
 {
  descr.dimNames = vec;
 }

void simUnitsData::addDim(string dimName, set<string> elements)
 {
  strVector tmp;
  set<string>::iterator it = elements.begin();
  while(it != elements.end()) {
    tmp.push_back(*it);
    it++;
  }
  descr.addDim(dimName,tmp);
 }

void simUnitsData::addDim(string dimName, set<int> elements)
 {
  strVector tmp;
  set<int>::iterator it = elements.begin();
  while(it != elements.end()) {
    tmp.push_back(IntToStr(*it));
    it++;
  }
  descr.addDim(dimName,tmp);
 }

void simUnitsData::addDim(string dimName, string element)
 {
  strVector tmp;
  tmp.push_back(element);
  descr.addDim(dimName,tmp);
 }

void simUnitsData::clear()
 {
  map<int, floatVector>::iterator it = data.begin();
  while (it != data.end()) {
    it->second.clear();
    it++;
  }
  data.clear();
  descr.clear();
 }

bool simUnitsData::SaveToFile(string outDir, string fileName)
{
	//****************************
	//**** Writimg *.MSU file ****
	//****************************
	map<int, floatVector>::iterator it = data.begin();
	if (it == data.end())
	{
		cout << "ERROR! Attempt to write empty data." << endl;
		return false;
	}
	ofstream f;
	fileName = outDir + pathSeparator + fileName;
	string fileNameTmp = fileName + ".msu";
	f.open(fileNameTmp.c_str(), ios::out | ios::binary);
	if (f.is_open()) {
    while (it != data.end())
	{
		int tmp = it->first;
		if (tmp < 0)
		{
			data.erase(tmp);
			it = data.begin();
		}
		else
			it++;
    }
    int numSimU = data.size();
    INV_BYTE_ORDER(numSimU);
    f.write(reinterpret_cast<char *>(&numSimU), sizeof(int));
	// writing simulation units to file
    it = data.begin();
    while (it != data.end())
	{
		int tmp = it->first;
        INV_BYTE_ORDER(tmp);
        f.write(reinterpret_cast<char *>(&tmp), sizeof(int));
		it++;
    }
    f.close();
    cout << "Successfully written to binary file: " << fileNameTmp << endl;
	} else {
		cout << "Unable to save to file!" << endl;
		return false;
	}
	//****************************
	//**** Writimg *.MDC file ****
	//****************************
	N = descr.getN();
	fileNameTmp = fileName + ".mdc";
	f.open(fileNameTmp.c_str(), ios::out | ios::binary);
	if (f.is_open()) {
		it = data.begin();
		while (it != data.end())
		{
			if (it->first >= 0)
			for (int i = 0; i < N; i++)
			{
				INV_BYTE_ORDER(it->second[i]);
				f.write(reinterpret_cast<char *>(&it->second[i]), sizeof(float));
			}
			it++;
		}
		f.close();
		cout << "Successfully written to binary file: " << fileNameTmp << endl;
	} else {
		cout << "Unable to save to file!" << endl;
		return false;
	}
	descr.SaveToFile(outDir + pathSeparator + "main", "MAP");
	return true;
}