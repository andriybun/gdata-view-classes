#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include "MDT.h"

using namespace std;

int main()
 {
//  MDT obj = MDT("data\\main.mdt");
//  cout << obj.getN() << endl;
//  vector<string> vec;
//  vec.push_back("- low input");
//  vec.push_back("grsg");
//  vec.push_back("FTP");
//  cout << obj.getHash(vec) << endl;
  MDT space;
  space.rename("G4M Output");
  space.addDimEl("Carbon price", "0");
  space.addDimEl("Year", "1990");
  space.addDimEl("Year", "2000");
  space.addDimEl("Year", "2010");
  space.addDimEl("Parameter", "Land area");
  space.addDimEl("Parameter", "Forest area");

//  vector<string> vec;
//  vec.push_back("0");
//  vec.push_back("- low input");
//  vec.push_back("- grsg");
//  vec.push_back("- FTP");
//  
//  cout << space.getHash(vec) << endl;

  space.SaveToFile("--del", "TABLE");
  system("pause");
 }
