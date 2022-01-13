#pragma once

#include <string>

class Resource {

private:
	void* address;

	unsigned int id;
	int category;
	std::string name;

public:
	Resource();
	virtual ~Resource();

	virtual void* getAddress();

};