#include "Death.h"


Death::~Death()
{
}


void Death::Enter(GameWorld& world)
{

	// Remove yourself from the board
	world.FreeUnitFromTile(unit_.GetCurrentTile());

	unit_.SetSpriteId(102); // Change Sprite

}


void Death::Step(GameWorld& world, float dt)
{

	// update the timer
	timer_ += dt;

	// Take action 
	if (timer_ > 0.5f)
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
