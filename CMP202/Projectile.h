#pragma once

#include "SFML\System\Vector2.hpp"
#include "GameWorld.h"
#include "GameObject.h"

class Projectile : public GameObject
{

public:

	Projectile(sf::Vector2f Start, sf::Vector2i TargetTile);
	~Projectile();

	SpriteObject& Update(GameWorld& world, float deltaTime);

private:

	// Start and end locations 
	sf::Vector2f direction;
	sf::Vector2f destination;
	sf::Vector2i destinationTile;

};

