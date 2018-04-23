#include "Projectile.h"


Projectile::Projectile(sf::Vector2f Start, sf::Vector2i TargetTile)
{

	// Set the starting location
	sprite_.screen_position = Start;
	// set the sprite 
	sprite_.id = 9;

	// Calculate the end location
	sf::Vector2f End = sf::Vector2f(TargetTile.x * TILESIZE, TargetTile.y * TILESIZE);

	// Calculate the direction to be applied 
	direction = (End - Start);

	// Normalise it
	direction = direction/(std::sqrtf(std::powf(direction.x, 2.0f) + std::powf(direction.y, 2.0f)));

	// set the destination
	destination = End;
	destinationTile = TargetTile;

}


Projectile::~Projectile()
{
}


SpriteObject& Projectile::Update(GameWorld& world, float deltaTime)
{

	// Move the unit in the direction of travel
	sprite_.screen_position = sprite_.screen_position + direction * PROJECTILESPEED * deltaTime;

	// whats the current vector direction
	sf::Vector2f currentDirection = destination - sprite_.screen_position;

	// Check if thedot product to see the unit is passed the point 
	if ((direction.x * currentDirection.x + direction.y * currentDirection.y) < 0.0f)
	{

		// Attack the unit on the tile
		world.AttackUnitOnTile(destinationTile, UNITATTACK);

		// Flag for deletion
		setActive(false);
		setDeleted(true);

	}

	// Still needs drawn
	return sprite_;

}
