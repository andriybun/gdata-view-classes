#include "baseData.h"

baseData::baseData(void)
{
}

baseData::~baseData(void)
{
}

//////////////////////////////////////////////////////////////////////////
// Private member
//////////////////////////////////////////////////////////////////////////

void baseData::setPartialHash()
{
	if (partialHash.empty())
	{
		partialHashOffset = 1;
		partialHash.push(partialHashOffset * descr.getCoordinate(0, point[0]));
	}
	else if (partialHash.top() < 0)
	{
		partialHash.push(-1);
	}
	else
	{
		partialHashOffset *= descr.dimCardinals[point.size()-2];
		partialHash.push(partialHash.top() + partialHashOffset * descr.getCoordinate(point.size()-1, point[point.size()-1]));
	}
}

//////////////////////////////////////////////////////////////////////////
// Methods to modify MDT description
//////////////////////////////////////////////////////////////////////////
void baseData::rename(std::string name)
{
	descr.paramName = name;
}

void baseData::renameDims(str_vector_t vec)
{
	descr.dimNames = vec;
}

void baseData::addDim(std::string dimName, str_vector_t elements)
{
	descr.addDim(dimName, elements);
}

void baseData::addDim(std::string dimName, int_vector_t elements)
{
	int_vector_t::iterator it = elements.begin();
	str_vector_t tmp;
	while (it != elements.end())
	{
		tmp.push_back(IntToStr(*it));
		it++;
	}
	descr.addDim(dimName, tmp);
}

//////////////////////////////////////////////////////////////////////////
// Methods to modify point
//////////////////////////////////////////////////////////////////////////

void baseData::pointPush(std::string val)
{
	point.push_back(val);
	setPartialHash();
}

void baseData::pointPush(int val)
{
	point.push_back(IntToStr(val));
	setPartialHash();
}

void baseData::pointPop()
{
	partialHashOffset /= (point.size() == 1 ? 1 : descr.dimCardinals[point.size()-2]);
	point.pop_back();
	partialHash.pop();
}

void baseData::pointClear()
{
	point.clear();
	partialHashOffset = 0;
	while (!partialHash.empty())
	{
		partialHash.pop();
	}

}
