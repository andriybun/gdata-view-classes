#include "tests.h"

void testTableData() 
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
}

void testSimUnitsMap()
{
	simUnitsMap sMap("data\\simu.bin.nik");
	cout << "SIMU corresponding to coordinates:" << endl;
	cout << sMap.getSimu(-34.458391368, 83.541597348) << endl;
	cout << sMap.getSimu(-33.791724968, 83.541597348) << endl;
	cout << sMap.getSimu(-179.9583332 + 594./12, -55.87501355 + 354./12) << endl;
	cout << "SUMU's per cell 0.5x0.5:" << endl;
	cout << sMap.simuPerCell(-179.9583332 + 594./12, -55.87501355 + 354./12) << endl;
	//sMap.saveToFile_ESRIGrid();
	cout << "SIMU statistics:" << endl;
	int xRes = 4320;             // Longitude resolution 
	int yRes = 1674;             // Latitude resolution
	float xMin = -179.9583332;   // Minimum longitude value
	float yMin = -55.87501355;   // Minimum latitude value
	int max = 0;
	int sum = 0;
	int min = 999;
	int quantity = 0;
	for (int i = 0; i < xRes; i++)
	{
		for (int j = 0; j < yRes; j++)
		{
			int tmp = sMap.simuPerCell(xMin + 0.5 * i, yMin + 0.5 * j);
			if (tmp)
			{
				if (tmp < min) min = tmp;
				if (tmp > max) max = tmp;
				sum += tmp;
				quantity++;
			}
		}
	}
	cout << "min = " << min << endl;
	cout << "max = " << max << endl;
	cout << "avg = " << 1. * sum / quantity << endl;

/*double v = 75;
double h = 275+1;
sMap.simuPerCell(-179.9583332+h, -55.87501355+v+1.5);
cout << endl;
sMap.simuPerCell(-179.9583332+h, -55.87501355+v+1);
cout << endl;
sMap.simuPerCell(-179.9583332+h, -55.87501355+v+0.5);
cout << endl;
*/
//  sMap.saveToFile();
}

void testSimUnitsMapNewFeatures()
{
	// Init simUnitsData object:
	simUnitsData ASU;
	ASU.rename("G4M parameters");

	// Helper set:
	set<int> years;
	years.insert(1990);

	vector<string> dimNames;
	dimNames.push_back("Scenario");
	dimNames.push_back("Year");
	dimNames.push_back("Results");

	// Adding dimensions' parameters:
	ASU.addDim(dimNames[0], "Baseline");
	ASU.addDim(dimNames[1], years);
	ASU.addDim(dimNames[2], "Test param");

	vector<string> point;
	point.push_back("Baseline");
	point.push_back("1990");
	point.push_back("Test param");

	simUnitsMap sMap("data\\simu.bin.nik");
	{
		for (int x = -20; x <= 20; x++)
		{
			for (int y = -20; y <= 20; y++)
			{
				vector<simUnitsMap::simu_info_struct_t> unitsInCell = sMap.getSimuInfoByXY(5+x/2.0, 45+y/2.0);
				cout << "Cell: " << endl;
				for (int i = 0; i < unitsInCell.size(); i++)
				{
					cout << "\t" << unitsInCell[i].simu << "\t- " << unitsInCell[i].simuFraction << endl;
					ASU.insert(unitsInCell[i].simu, unitsInCell[i].simuFraction * 100, point);
				}
			}
		}
	}
	ASU.renameDims(dimNames);
	ASU.SaveToFile("D:\\Workspace\\IIASA\\GLOBIOM GUI\\data\\maps", "my_test_map");
}

void testSimUnitsData()
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
  obj.SaveToFile("data", "-MYREG");
  obj.clear();
  obj.SaveToFile("data", "myRegion");
}

void testMDT()
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
}

void testActiveSimUnits()
{
  activeSimUnits obj;
  obj.insert(1);
  obj.insert(7);
  obj.insert(3);
  obj.insert(5);
  obj.insert(2);
  obj.insert(1);
  obj.saveToFile("myRegion");
}