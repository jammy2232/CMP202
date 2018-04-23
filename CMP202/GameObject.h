#pragma once

#include "SpriteRenderer.h"

class GameWorld;
struct SpriteObject;

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
	virtual SpriteObject& Update(GameWorld& world, float deltaTime) = 0;

	// Update the sprite info
	void SetScreenPosition(sf::Vector2f position);
	void SetSpriteId(int id);

private:

	// internal bools
	bool active_;
	bool deleted_;

protected:

	// Sprite Rendering Information 
	SpriteObject sprite_;

};

