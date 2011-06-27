#include "simUnitsData.h"

// default constructor
simUnitsData::simUnitsData()
{
	sMap = simUnitsMap();
	descr = MDT();
}

// constructors
simUnitsData::simUnitsData(std::string fileNameSimuBin)
{
	sMap = simUnitsMap(fileNameSimuBin);
	descr = MDT();
}

simUnitsData::simUnitsData(std::string fileNameSimuBin, std::string fileNameMdt)
{
	descr = MDT(fileNameMdt);
}

// destructor
simUnitsData::~simUnitsData()
{
}

//================
// Class methods:
//================

void simUnitsData::setMap(std::string fileNameSimuBin)
{
	sMap = simUnitsMap(fileNameSimuBin);
}

bool simUnitsData::insert(int SIMU, float val)
{
	std::map<int, float_vector_t>::iterator it = data.find(SIMU);
	if (it == data.end())
	{
		float_vector_t tmp;
		it = data.insert(make_pair(SIMU, tmp)).first;
	}
	int card = partialHash.top();
	if (card < 0)
	{
		return false;
	}
	if (it->second.size() < (card + 1))
	{
		it->second.resize(card + 1);
	}
	it->second[card] = val;
	return true;
}

bool simUnitsData::insert(double x, double y, float val, distribute_value_t distribute_value)
{
	if (partialHash.top() < 0) return false;
	bool result = false;
	std::vector<simUnitsMap::simu_info_struct_t> unitsInCell;
	sMap.getSimuInfoByXY(x, y, unitsInCell);
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

bool simUnitsData::insert(double x, double y, float val, std::string paramName, distribute_value_t distribute_value)
{
	if (partialHash.top() < 0) return false;
	bool result = false;
	if (point.size() == descr.nDims) pointPop();
	pointPush(paramName);
	std::vector<simUnitsMap::simu_info_struct_t> unitsInCell;
	sMap.getSimuInfoByXY(x, y, unitsInCell);
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

bool simUnitsData::insertByLastCoord(int SIMU, int coord, float val)
{
	assert((point.size()+1) == descr.getNDims());
	if (partialHash.top() < 0) return false;
	std::map<int, float_vector_t>::iterator it = data.find(SIMU);
	if (it == data.end())
	{
		float_vector_t tmp;
		it = data.insert(make_pair(SIMU, tmp)).first;
	}
	long long card = partialHash.top() + partialHashOffset * descr.dimCardinals[point.size()-1] * coord;
	if (card < 0) return false;
	if (it->second.size() < (card + 1)) it->second.resize(card + 1);
	it->second[card] = val;
	return true;
}

bool simUnitsData::insertByLastCoord(double x, double y, int coord, float val, distribute_value_t distribute_value)
{
	assert((point.size()+1) == descr.getNDims());
	if (partialHash.top() < 0) return false;
	bool result = false;
	std::vector<simUnitsMap::simu_info_struct_t> unitsInCell;
	sMap.getSimuInfoByXY(x, y, unitsInCell);
	for (int i = 0; i < unitsInCell.size(); i++)
	{
		switch (distribute_value)
		{
		case DISTRIBUTE_PROPORTIONALLY:
			result = insertByLastCoord(unitsInCell[i].simu, coord, unitsInCell[i].simuFraction * val);
			break;
		case IS_CONSTANT:
			result = insertByLastCoord(unitsInCell[i].simu, coord, val);
			break;
		default:
			break;
		}
	}
	return true;
}

simUnitsMap & simUnitsData::getSimUnitsMap()
{
	return sMap;
}

void simUnitsData::clear()
{
	std::map<int, float_vector_t>::iterator it = data.begin();
	while (it != data.end())
	{
		it->second.clear();
		it++;
	}
	data.clear();
	descr.clear();
}

bool simUnitsData::SaveToFile(std::string outDir, std::string fileName)
{
	//****************************
	//**** Writimg *.MSU file ****
	//****************************
	std::map<int, float_vector_t>::iterator it = data.begin();
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
	long long N = descr.getN();
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
						INV_BYTE_ORDER(val);
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