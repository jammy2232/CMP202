#include "AiState.h"
#include "Unit.h"

#include <assert.h>

AiState::~AiState()
{
}


bool AiState::MoveTheUnit(float dt)
{
	
	// Switch for each step of the motion
	switch (move_)
	{

	case CHECK:

		if (MovePossible())
		{
			blocked = false;
			move_ = PREPARE;
			return false;
		}
		else
		{
			move_ = BLOCKED;
			return false;
		}

		break;

	case PREPARE:

		calculateMovementVector();
		move_ = MOVING;
		return false;

		break;

	case MOVING:

		if (incrementMovement(dt))
		{
			move_ = CHECK;
			return true;
		}

		break;

	case BLOCKED:

		if (wait(dt))
		{

			// waiting to see if the space becomes available in a certain time
			move_ = CHECK;
			return false;

		}
		else
		{
			// wait time exceeded
			blocked = true;
			// Reset all the logic
			move_ = CHECK;
			return false;

		}

		break;

	}


	return false;

}


void AiState::calculateMovementVector()
{

	// calculate movement 
	float legnth = std::sqrtf(std::powf((unit_->GetGoal().x - unit_->spriteInfo.x), 2.0f) + std::powf((unit_->GetGoal().y - unit_->spriteInfo.y), 2.0f));
	float xdir = (unit_->GetGoal().x - unit_->spriteInfo.x) / legnth;
	float ydir = (unit_->GetGoal().y - unit_->spriteInfo.y) / legnth;
	// Calculate the movement vector
	forwardDirection = sf::Vector2f(xdir, ydir);

}


bool AiState::MovePossible()
{

	// Is the next path step available
	if (!(unit_->gameBoard)[currentDestination.y * unit_->mapSize() + currentDestination.x])
	{

		// Updates the tile map
		assert((unit_->gameBoard)[currentDestination.y * unit_->mapSize() + currentDestination.x] == nullptr);

		// Set the new tile to taken
		(unit_->gameBoard)[currentDestination.y * unit_->mapSize() + currentDestination.x] = unit_;
		// release the old tile
		(unit_->gameBoard)[unit_->currentTile.y * unit_->mapSize() + unit_->currentTile.x] = nullptr;

		// returns success
		return true;

	}

	// Move isn't possible
	return false;

}


bool AiState::incrementMovement(float dt)
{

	// bools to check
	bool xComplete = false;
	bool yComplete = false;

	// Check if you have reached the x destination
	if (forwardDirection.x != 0.0f)
	{

		unit_->spriteInfo.x += forwardDirection.x * UNITSPEED * dt * speedMod_;

		if (std::abs(unit_->spriteInfo.x - unit_->GetGoal().x) < UNITSPEED * dt * speedMod_)
		{
			unit_->spriteInfo.x = unit_->GetGoal().x;
			xComplete = true;
		}
	}
	else
	{
		xComplete = true;
	}

	// Check if you have reached the y destination
	if (forwardDirection.y != 0.0f)
	{

		unit_->spriteInfo.y += forwardDirection.y * UNITSPEED * dt * speedMod_;

		if (std::abs(unit_->spriteInfo.y - unit_->GetGoal().y) < UNITSPEED * dt * speedMod_)
		{
			unit_->spriteInfo.y = unit_->GetGoal().y;
			yComplete = true;
		}
	}
	else
	{
		yComplete = true;
	}


	// Flag for render update 
	unit_->posDirty_ = true;


	// Transition to check for enemies each step
	if (yComplete && xComplete)
	{
		unit_->spriteInfo.y = unit_->GetGoal().y;
		unit_->spriteInfo.x = unit_->GetGoal().x;
		unit_->currentTile = unit_->GetDestination();
		unit_->UpdateDestination();
		return true;
	}

	// Movement not complete 
	return false;

}




bool AiState::wait(float dt)
{
	
	blockingtime += dt;

	// Wait upto 10 seconds blocked
	if (blockingtime > 0.5f)
	{
		blockingtime = 0.0f;
		return false;
	}

	return true;

}

