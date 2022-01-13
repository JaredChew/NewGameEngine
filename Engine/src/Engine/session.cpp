#include "Session.h"

Session::Session() {

	nextSession = nullptr;

}

Session::~Session() {

	delete nextSession;
	nextSession = nullptr;

}

void Session::preUpdate() { }

void Session::postUpdate() { }

void Session::render() { }

void Session::setNextSession(Session *nextSession) {
	this->nextSession = nextSession;
}

Session *const Session::getNextSession() {

	Session *nextSession = this->nextSession;

	this->nextSession = nullptr;

	return nextSession;

}