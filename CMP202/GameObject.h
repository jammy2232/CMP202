#pragma once

#include "RenderManager.h"


class GameObject
{

public:

	GameObject();
	~GameObject();

	// functions to control activity
	bool isActive();
	void setActive(bool active);

	// Functions to indicate objects for cleanup
	bool isDeleted();
	void setDeleted(bool active);

	// virtual functions for processing gameobjects
	virtual SpriteObject& Update(float deltaTime);

private:

	// internal bools
	bool active;
	bool deleted;

};

