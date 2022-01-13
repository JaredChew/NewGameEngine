#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Session {

private:

	Session *nextSession;

protected:

	void setNextSession(Session *nextSession);

public:

	Session();

	Session* const getNextSession();

	virtual ~Session();

	virtual void preUpdate();
	virtual void update() = 0;
	virtual void postUpdate();

	virtual void render();

};