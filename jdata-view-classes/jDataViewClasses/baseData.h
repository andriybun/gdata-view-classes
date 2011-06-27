#ifndef BASE_DATA_H_
#define BASE_DATA_H_

#include <vector>
#include <stack>
#include <string>
#include <set>

#include "MDT.h"

class baseData
{
protected:
	typedef std::vector<std::string> str_vector_t;
	typedef std::vector<int> int_vector_t;
	typedef std::vector<float> float_vector_t;
	MDT descr;
	str_vector_t point;
	std::stack<long long> partialHash;
	long long partialHashOffset;
	void setPartialHash();
public:
	baseData(void);
	~baseData(void);
	// Rename dataset:
	void rename(std::string name);
	// Rename dimensions of the dataset:
	void renameDims(str_vector_t vec);
	// Add new dimension:
	void addDim(std::string dimName, str_vector_t elements);
	void addDim(std::string dimName, int_vector_t elements);
	// Set values for point:
	void pointPush(std::string val);
	void pointPush(int val);
	void pointPop();
	void pointClear();
};

#endif