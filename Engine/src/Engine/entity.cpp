#include "entity.h"

#include "transform.h"

Entity::Entity() {

	transform = new Transform();

	id = 0; //generate unique id, save it to array generated uniqed id list

}

Entity::~Entity() {

	child.clear();

	delete transform;
	transform = nullptr;

	//erase id from stored generated id list

}

int Entity::getID() {

	return id;

}