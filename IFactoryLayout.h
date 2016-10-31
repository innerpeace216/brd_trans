#pragma once
#include <string>
#include "Layout.h"

class IFactory{
public:
	IFactory(){};
	IFactory(std::string &file_name){};
	virtual psiv::Layout* generateLayout(std::string &file_name);
	virtual ~IFactory(){};


}