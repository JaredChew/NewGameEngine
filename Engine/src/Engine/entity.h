#pragma once

#include <list>
#include <string>

class Transform;

class Entity {

private:
	unsigned int id;

public:
	Entity* parent;
	std::list<Entity*> child;

	Transform *transform;
	
	int group;
	std::string name;

public:
	Entity();
	virtual ~Entity();

	int getID();

};