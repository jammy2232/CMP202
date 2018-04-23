#include "Death.h"


Death::~Death()
{
}


void Death::Enter(GameWorld& world)
{

	unit_.SetSpriteId(102); // Change Sprite

	// Remove yourself from the board
	world.FreeUnitFromTile(unit_.GetCurrentTile());

}


void Death::Step(GameWorld& world, float dt)
{

	// update the timer
	timer_ += dt;

	// Take action 
	if (timer_ > 10.0f)
	{

		unit_.setActive(false);
		unit_.setDeleted(true);

		// Add a memory cleanup function
		return;

	}

}


void Death::Exit(GameWorld& world)
{

	// Never Reached

}
