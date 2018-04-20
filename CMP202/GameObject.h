#pragma once

#include "SpriteRenderer.h"

class GameObject
{

public:

	GameObject();
	~GameObject();

	// functions to control activity
	bool isActive() { return active_; }
	void setActive(bool active) { active_ = active; }

	// Functions to indicate objects for cleanup
	bool isDeleted() { return deleted_;	}
	void setDeleted(bool deleted) {	deleted_ = deleted;	}

	// virtual functions for processing gameobjects
	virtual SpriteObject& Update(float deltaTime) { return SpriteObject(); }

private:

	// internal bools
	bool active_;
	bool deleted_;

};

