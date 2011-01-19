#include "griddataLite.h"

template <class TP>
void griddataLite<TP>::ShowArray()
{
	TP val;
	int isChar = 0;
	char t1;
	unsigned char t2;
	if ((typeid(val).name() == typeid(t1).name()) || 
		(typeid(val).name() == typeid(t2).name())) isChar = 1;
	for (int j = 0; j < VerResolution; j++)
	{
		cout << j << "|\t";
		for (int i = 0; i < HorResolution; i++)
		{
			if (isChar)
				cout << (int)grid[j*HorResolution+i] << "\t";
			else
				cout << grid[j*HorResolution+i] << "\t";
		}
		cout << endl;  
	}   
}

template <class TP>
void griddataLite<TP>::PrintToFile(string fileName, string rastrType = "ESRI")
{
	TP val;
	int isChar = 0;
	char t1;
	unsigned char t2;
	if ((typeid(val).name() == typeid(t1).name()) ||
		(typeid(val).name() == typeid(t2).name())) isChar = 1;
	ofstream f;
	fileName = "output\\" + fileName + ".asc";
	f.open(fileName.c_str(),ios::out);
	if (f.is_open())
	{
		/////////////////////// Select grid type:
		if (rastrType == "GRASS")
		{
			f << "cols: " << HorResolution << endl;
			f << "rows: " << VerResolution << endl;
			f << "west: -180" << endl;
			f << "south: -90" << endl;
			f << "north: 90" << endl;
			f << "east: 180" << endl;
		}
		else if (rastrType == "ESRI")
		{
			//----------------------------    
			// ESRI ascii Grid
			f << "NCOLS " << HorResolution << endl;
			f << "NROWS " << VerResolution << endl;
			f << "XLLCORNER -180" << endl;
			f << "YLLCORNER -90" << endl;
			f << "CELLSIZE " << 360./HorResolution  << endl;
			f << "NODATA_VALUE -9999" << endl;
		}
		else
		{
			cout<<"griddataLite3 error message: Specify correct rastrType: ESRI or GRASS"<<endl;
		}
		//-----------------------------        
		for (int j = 0; j < VerResolution; j++)
		{
			for (int i = 0; i < HorResolution; i++)
			{
				if (isChar)
					f << (int)grid[(VerResolution-j-1)*HorResolution+i] << " ";
				else
					f << grid[(VerResolution-j-1)*HorResolution+i] << " ";
			}
			f << endl;  
		}
		f.close(); 
	}
	else
	{
		cout << "Unable to save to file!" << endl;
	}
}

template <class TP>
void griddataLite<TP>::set(int x, int y, TP val)
{
	grid[y*HorResolution+x] = val;
}

template <class TP>
void griddataLite<TP>::inc(int x, int y, TP val)
{
	grid[y*HorResolution+x] += val;
}

template <class TP>
TP griddataLite<TP>::get(int x, int y)
{
	return (grid[y*HorResolution+x]);
}

// Class constructor
template <class TP>
griddataLite<TP>::griddataLite(int HR, int VR, TP val)
{
	HorResolution = HR;
	VerResolution = VR;
	grid = new TP[HorResolution*VerResolution];
	for (int j = 0; j < VerResolution; j++)
		for (int i = 0; i < HorResolution; i++)
		{
			grid[j*HorResolution+i] = val;
		}
}

// Default constructor
template <class TP>
griddataLite<TP>::griddataLite()
{
	HorResolution = 720;
	VerResolution = 360;
	TP val = 0;
	grid = new TP[HorResolution*VerResolution];
	for (int j = 0; j < VerResolution; j++)
		for (int i = 0; i < HorResolution; i++)
		{
			grid[j*HorResolution+i] = val;
		}
}

// Copy constructor
template <class TP>
griddataLite<TP>::griddataLite(const griddataLite& g)
{
	HorResolution = g.HorResolution;
	VerResolution = g.VerResolution;
	grid = new TP[HorResolution*VerResolution];
	memcpy(grid,g.grid,HorResolution*VerResolution*sizeof(TP));
}

// Destructor
template <class TP>
griddataLite<TP>::~griddataLite()
{
	delete []grid;
}