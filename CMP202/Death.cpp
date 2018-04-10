#include "Death.h"


Death::~Death()
{
}


void Death::Enter()
{

	unit_->spriteInfo.id = 102; // Checnge sprite
	unit_->posDirty_ = true;

}


void Death::Step(float dt)
{

	// update the timer
	timer += dt;

	// Take action 
	if (timer > 10.0f)
	{

		// Remove yourself from the board
		unit_->gameBoard[unit_->currentTile.y * unit_->mapSize() + unit_->currentTile.x] = nullptr;
		unit_->Active_ = false;

		// Add a memory cleanup function


		return;
	}

}


void Death::Exit()
{
}
