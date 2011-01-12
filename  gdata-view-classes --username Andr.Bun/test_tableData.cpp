#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <iomanip>
#include "tableData.h"

using namespace std;

int main()
 {
  vector<string> point1 = vector<string>(5, "D");
  vector<string> point2 = vector<string>(4, "D");
  vector<string> point3 = vector<string>(3, "D");
  cout << "Dimension 1:" << endl;
  for (int i = 0; i < point1.size(); i++) {
    point1[i] =  point1[i] + "_" + char('a' + i);
    cout << point1[i] << endl;
  }
  cout << "Dimension 2:" << endl;
  for (int i = 0; i < point2.size(); i++) {
    point2[i] =  point2[i] + "_" + char('1' + i);
    cout << point2[i] << endl;
  }
  cout << "Dimension 3:" << endl;
  for (int i = 0; i < point3.size(); i++) {
    point3[i] =  point3[i] + "_" + char('z' - i);
    cout << point3[i] << endl;
  }
  vector<string> point;
  tableData obj;
  for (int i = 0; i < point1.size(); i++) {
    point.push_back(point1[i]);
    for (int j = 0; j < point2.size(); j++) {
      point.push_back(point2[j]);
      for (int k = 0; k < point3.size(); k++) {
        point.push_back(point3[k]);
        obj.insert(((i + 1)*(j + 1)*(k + 1)) % 12, point);
        point.pop_back();
      }
      point.pop_back();
    }
    point.pop_back();
  }
  obj.SaveToFile("data", "DemandCrops_gui");
  system("pause");
 }
