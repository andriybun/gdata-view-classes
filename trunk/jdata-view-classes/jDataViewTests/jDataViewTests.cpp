// jDataViewTests.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iomanip>

#include "tests.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int v = 4;
	testTableData();
	//testTableDataRead();
	//testSimUnitsMap();
	testSimUnitsMapNewFeatures();
	//testSimUnitsData();
	//testMdt();
	//testActiveSimUnits();
	return 0;
}

