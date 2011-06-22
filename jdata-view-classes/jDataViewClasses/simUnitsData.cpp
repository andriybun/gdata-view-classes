#include "simUnitsData.h"

// default constructor
simUnitsData::simUnitsData()
{
	sMap = simUnitsMap();
	descr = MDT();
	N = descr.getN();
}

// constructors
simUnitsData::simUnitsData(string fileNameSimuBin)
{
	sMap = simUnitsMap(fileNameSimuBin);
	descr = MDT();
	N = descr.getN();
}

simUnitsData::simUnitsData(string fileNameSimuBin, string fileNameMdt)
{
	//sMap = simUnitsMap(fileNameSimuBin);
	descr = MDT(fileNameMdt);
	N = descr.getN();
}

// destructor
simUnitsData::~simUnitsData()
{
	/* map<int, float_vector_t>::iterator it = data.begin();
	while (it != data.end()) {
	delete it->second;
	it++;
	} */
}

//================
// Class methods:
//================

//bool simUnitsData::insert(int SIMU, float val)
//{
//	if (data.find(SIMU) == data.end())
//	{
//		float_vector_t tmp;
//		data.insert(make_pair(SIMU, tmp));
//	}
//	int card = descr.getHash(point);
//	if (data[SIMU].size() < (card + 1)) data[SIMU].resize(card + 1);
//	// TODO: add exceptions
//	if (card < 0)
//	{
//		return false;
//	}
//	data[SIMU][card] = val;
//	return true;
//}

bool simUnitsData::insert(int SIMU, float val)
{
	map<int, float_vector_t>::iterator it = data.find(SIMU);
	if (it == data.end())
	{
		float_vector_t tmp;
		it = data.insert(make_pair(SIMU, tmp)).first;
	}
	//int card = descr.getHash(point);
	int card = partialHash.top();
	if (it->second.size() < (card + 1))
	{
		it->second.resize(card + 1);
	}
	if (card < 0)
	{
		return false;
	}
	it->second[card] = val;
	return true;
}

bool simUnitsData::insert(double x, double y, float val, distribute_value_t distribute_value)
{
	bool result = false;
	vector<simUnitsMap::simu_info_struct_t> unitsInCell = sMap.getSimuInfoByXY(x, y);
	for (int i = 0; i < unitsInCell.size(); i++)
	{
		switch (distribute_value)
		{
			case DISTRIBUTE_PROPORTIONALLY:
				result = insert(unitsInCell[i].simu, unitsInCell[i].simuFraction * val);
				break;
			case IS_CONSTANT:
				result = insert(unitsInCell[i].simu, val);
				break;
			default:
				break;
		}
	}
	return result;
}

bool simUnitsData::insert(double x, double y, float val, string paramName, distribute_value_t distribute_value)
{
	bool result = false;
	if (point.size() == descr.nDims) pointPop();
	pointPush(paramName);
	vector<simUnitsMap::simu_info_struct_t> unitsInCell = sMap.getSimuInfoByXY(x, y);
	for (int i = 0; i < unitsInCell.size(); i++)
	{
		switch (distribute_value)
		{
		case DISTRIBUTE_PROPORTIONALLY:
			result = insert(unitsInCell[i].simu, unitsInCell[i].simuFraction * val);
			break;
		case IS_CONSTANT:
			result = insert(unitsInCell[i].simu, val);
			break;
		default:
			break;
		}
	}
	pointPop();
	return result;
}

bool simUnitsData::insertByHash(int SIMU, int card, float val)
{
	map<int, float_vector_t>::iterator it = data.find(SIMU);
	if (it == data.end())
	{
		float_vector_t tmp;
		it = data.insert(make_pair(SIMU, tmp)).first;
	}
	if (it->second.size() < (card + 1)) it->second.resize(card + 1);
	it->second[card] = val;
	return true;
}

void simUnitsData::setMap(string fileNameSimuBin)
{
	sMap = simUnitsMap(fileNameSimuBin);
}

void simUnitsData::rename(string name)
 {
  descr.paramName = name;
 }

void simUnitsData::renameDims(str_vector_t vec)
 {
  descr.dimNames = vec;
 }

void simUnitsData::addDim(string dimName, set<string> elements)
{
	str_vector_t tmp;
	set<string>::iterator it = elements.begin();
	while(it != elements.end())
	{
		tmp.push_back(*it);
		it++;
	}
	descr.addDim(dimName,tmp);
}

void simUnitsData::addDim(string dimName, set<int> elements)
{
	str_vector_t tmp;
	set<int>::iterator it = elements.begin();
	while(it != elements.end())
	{
		tmp.push_back(IntToStr(*it));
		it++;
	}
	descr.addDim(dimName,tmp);
}

void simUnitsData::addDim(string dimName, string element)
{
	str_vector_t tmp;
	tmp.push_back(element);
	descr.addDim(dimName,tmp);
}

void simUnitsData::pointPush(string val)
{
	point.push_back(val);
	setPartialHash();
}

void simUnitsData::pointPush(int val)
{
	point.push_back(IntToStr(val));
	setPartialHash();
}

void simUnitsData::setPartialHash()
{
	if (partialHash.empty())
	{
		partialHashOffset = 1;
		partialHash.push(partialHashOffset * descr.getCoordinate(0, point[0]));
	}
	else
	{
		partialHashOffset *= descr.dimCardinals[point.size()-2];
		partialHash.push(partialHash.top() + partialHashOffset * descr.getCoordinate(point.size()-1, point[point.size()-1]));
	}
}

void simUnitsData::pointPop()
{
	partialHashOffset /= descr.dimCardinals[point.size()-2];
	point.pop_back();
	partialHash.pop();
}

void simUnitsData::pointClear()
{
	point.clear();
	partialHashOffset = 0;
	while (!partialHash.empty())
	{
		partialHash.pop();
	}
	
}

void simUnitsData::clear()
{
	map<int, float_vector_t>::iterator it = data.begin();
	while (it != data.end())
	{
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
	map<int, float_vector_t>::iterator it = data.begin();
	if (it == data.end())
	{
		cout << fileName << ": Error! Attempt to write empty data." << endl;
		return false;
	}
	ofstream f;
	fileName = outDir + pathSeparator + fileName;
	string fileNameTmp = fileName + ".msu";
	f.open(fileNameTmp.c_str(), ios::out | ios::binary);
	if (f.is_open())
	{
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
	}
	else
	{
		cout << "Unable to save to file!" << endl;
		return false;
	}
	//****************************
	//**** Writimg *.MDC file ****
	//****************************
	N = descr.getN();
	printf("N = %d\ndata.size() = %d\n", N, data.size());
	fileNameTmp = fileName + ".mdc";
	f.open(fileNameTmp.c_str(), ios::out | ios::binary);
	if (f.is_open())
	{
		it = data.begin();
		int bytesWritten = 0;
		while (it != data.end())
		{
			if (it->first >= 0)
			{
				for (int i = 0; i < N; i++)
				{
					float val;
					if (it->second.size() < i + 1)
					{
						val = (float)0;
					}
					else
					{
						val = it->second[i];
						INV_BYTE_ORDER(it->second[i]);
					}
					f.write(reinterpret_cast<char *>(&val), sizeof(float));
				}
				bytesWritten += N * sizeof(float);
			}
			it++;
		}
		printf("written %d bytes to mdc-file\n", bytesWritten);
		f.close();
		cout << "Successfully written to binary file: " << fileNameTmp << endl;
	}
	else
	{
		cout << "Unable to save to file!" << endl;
		return false;
	}
	descr.SaveToFile(outDir + pathSeparator + "main", "MAP");
	return true;
}