#include "tableData.h"

// default constructor
tableData::tableData()
{
	descr = MDT();
	N = descr.getN();
}

// constructor
tableData::tableData(string fileNameGdc)
{
	ifstream f;
	f.open(fileNameGdc.c_str(), ios::in | ios::binary);
	if (f.is_open())
	{
		// Reading dimensions info:
		int nDims;
		f.read(reinterpret_cast<char *>(&nDims), sizeof(int));
		INV_BYTE_ORDER(nDims);
		int_vector_t dimCardinals;
		dimCardinals.resize(4);
		for (int i = 0; i < nDims; i++)
		{
			int tmp;
			f.read(reinterpret_cast<char *>(&tmp), sizeof(int));
			INV_BYTE_ORDER(tmp);
			dimCardinals[i] = tmp;
		}
		// Reading *.gdt file
		string fileNameGdt;
		if(fileNameGdc.find_last_of(".") != std::string::npos)
		{
			fileNameGdt = fileNameGdc.substr(0, fileNameGdc.find_last_of(".") + 1) + "gdt";
		}
		else
		{
			// TODO: add error
		}
		descr = MDT(fileNameGdt, dimCardinals);
		// Number of non-zero records in the dataset
		int numRecords;
		f.read(reinterpret_cast<char *>(&numRecords), sizeof(int));
		INV_BYTE_ORDER(numRecords);
		// Reading data to buffers (still little endian)
		long long * hashTable = new long long [numRecords];
		f.read(reinterpret_cast<char *>(hashTable), sizeof(long long) * numRecords);
		float * valuesTable = new float [numRecords];
		f.read(reinterpret_cast<char *>(valuesTable), sizeof(float) * numRecords);
		// Processing buffers
		for (int i = 0; i < numRecords; i++)
		{
			INV_BYTE_ORDER(valuesTable[i]);
			INV_BYTE_ORDER(hashTable[i]);
			data.insert(make_pair(hashTable[i], valuesTable[i]));
		}
		cout << "Successfully read table from binary file: " << fileNameGdc << endl;
	}
	else
	{
		cout << "Unable to read file!" << endl;
		// TODO: add error
	}
	// TODO: add execution OK
}

// destructor
tableData::~tableData()
{
/*  map<int, float_vector_t>::iterator it = data.begin();
  while (it != data.end()) {
    delete it->second;
    it++;
  }*/
}

//================
// Class methods:
//================

void tableData::insert(float val, str_vector_t point)
{
	if (val != 0)
	{
		data.insert(make_pair(descr.getHash(point), val));
	}
}

void tableData::update(float val, str_vector_t point)
{
	if (val != 0)
	{
		long long card = descr.getHash(point);
		data.erase(card);
		data.insert(make_pair(card, val));
	}
}

void tableData::rename(string name)
{
	descr.paramName = name;
}

void tableData::renameDims(str_vector_t vec)
{
	descr.dimNames = vec;
}

void tableData::addDim(string dimName, set<string> elements)
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

void tableData::addDim(string dimName, set<int> elements)
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

void tableData::addDim(string dimName, string element)
{
	str_vector_t tmp;
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
	if (data.size() == 0)
	{
		cout << "ERROR! Attempt to write empty data." << endl;
		return false;
	}
	fileName = outDir + pathSeparator + fileName;
	string fileNameTmp = fileName + ".gdc";
	ofstream f;
	f.open(fileNameTmp.c_str(), ios::out | ios::binary);
	if (f.is_open())
	{
		// Writing info about dimensions:
		int tmp_int = descr.getNDims();
		INV_BYTE_ORDER(tmp_int);
		f.write(reinterpret_cast<char *>(&tmp_int), sizeof(int));
		for (int i = 0; i < descr.nDims; i++)
		{
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
		while (it != data.end())
		{
			long long tmp_long = it->first;
			INV_BYTE_ORDER(tmp_long);
			vl[i] = tmp_long;
			float tmp_float = it->second;
			INV_BYTE_ORDER(tmp_float);
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
	}
	else
	{
		cout << "Unable to save to file!" << endl;
		return false;
	}
	//****************************
	//**** Writimg *.GDT file ****
	//****************************
	descr.SaveToFile(fileName + ".gdt", "TABLE");
	return true;
}
