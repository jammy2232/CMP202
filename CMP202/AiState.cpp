#include "AiState.h"
#include "Unit.h"

AiState::~AiState()
{
}


bool AiState::MoveTheUnit(GameWorld& world, float dt)
{
	
	// Switch for each step of the motion
	switch (move_)
	{

	case CHECK:

		if (MovePossible(world))
		{
			// Can actually move
			blocked = false;
			move_ = PREPARE;
			// still atttempting move
			return false;
		}
		else
		{
			move_ = BLOCKED;
			// still atttempting move
			return false;
		}

		break;

	case PREPARE:

		// Prpare
		calculateMovementVector();
		move_ = MOVING;
		// still atttempting move
		return false;

		break;

	case MOVING:

		if (incrementMovement(dt))
		{
			// reset movement
			move_ = CHECK;
			// move complete
			return true;
		}

		break;

	case BLOCKED:

		if (wait(dt))
		{

			// waiting to see if the space becomes available in a certain time
			move_ = CHECK;
			// still atttempting move
			return false;

		}
		else
		{
			// wait time exceeded
			blocked = true;
			// Reset all the logic
			move_ = CHECK;
			// still atttempting move
			return false;

		}

		break;

	}

	// Catch all 
	return false;

}


void AiState::calculateMovementVector()
{

	// calculate forward direction 
	sf::Vector2f direction = unit_.GetPointDestination() - unit_.GetCurrentPoint();
	// Normailise the vector vector/length
	forwardDirection = direction / (std::sqrtf(std::powf(direction.x, 2.0f) + std::powf(direction.y, 2.0f)));

}


bool AiState::MovePossible(GameWorld& world)
{

	if (world.CheckForUnit(sf::Vector2i(unit_.GetTileDestination())))
	{
		// If there is a unit in the destination of this unit the movement isn't possible 
		return false;
	}

	// if it's possible update the world with this units intention to move
	// Set the new tile to taken
	if (!world.SetUnitOnTile(&unit_, unit_.GetTileDestination()))
	{
		assert(false);
	}

	// release the old tile
	world.FreeUnitFromTile(unit_.GetCurrentTile());

	// Set the units position 
	unit_.SetCurrentTile(unit_.GetTileDestination());

	// flag success
	return true;

}


bool AiState::incrementMovement(float dt)
{

	// Move the unit in the direction of travel
	unit_.SetScreenPosition(unit_.GetCurrentPoint() + forwardDirection * UNITSPEED * dt);

	// Check if the unit has reached the destination 

	// whats the current vector direction
	sf::Vector2f currentDirection = unit_.GetPointDestination() - unit_.GetCurrentPoint();

	// Check if thedot product to see the unit is passed the point 
	if ((forwardDirection.x * currentDirection.x + forwardDirection.y * currentDirection.y) < 0.0f)
	{

		// the unit is pasted the intended position so therefore set the position
		unit_.SetScreenPosition(unit_.GetPointDestination());
		unit_.UpdateDestination();
		return true;

	}

	// otherwise the move is not complete
	return false;

}


bool AiState::wait(float dt)
{
	
	blockingtime += dt;

	// Wait upto 0.5 seconds blocked
	if (blockingtime > 0.5f)
	{
		blockingtime = 0.0f;
		return false;
	}

	// still waiting
	return true;

}

