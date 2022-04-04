#ifndef HashFunc_H
#define HashFunc_H

#include <ctime>
#include "consistentHash/HashFunc.h"

class MD5HashFunc:public HashFunc 
{
public:
	virtual unsigned int GetKey(std::string str) override;
};

#endif