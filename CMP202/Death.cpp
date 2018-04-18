#include "Death.h"


Death::~Death()
{
}


void Death::Enter()
{

	unit_->SetSpriteId(102); // Checnge sprite
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
		unit_->world_.FreeUnitFromTile(unit_->GetCurrentTile());
		// unit_->Active_ = false;

		// Add a memory cleanup function
		return;

	}

}


void Death::Exit()
{
}
