#include "Moving.h"

Moving* Moving::stateInstance = nullptr;


Moving::Moving()
{
}


Moving::~Moving()
{
}


void Moving::Enter(Unit * unit)
{

	// Check if there is a vaild path
	if (unit->WaitngPath())
	{
		unit->wait = true;
		return;
	}
	else
	{
		unit->wait = false;
	}

	// Check if the unit is blocked
 	if (battleSceneReference->CheckIfTileIsOccupied(unit->GetDestination()))
	{
		unit->wait = true;
		return;
	}
	else
	{
		unit->wait = false;
	}

	// if your free to move work out the movement speed and update your position for other units
	if (!unit->wait)
	{
		// calculate movement 
		float legnth = std::sqrtf(std::powf((unit->GetGoal().x - unit->currentPosition.x), 2.0f) + std::powf((unit->GetGoal().y - unit->currentPosition.y), 2.0f));
		float xdir = (unit->GetGoal().x - unit->currentPosition.x) / legnth;
		float ydir = (unit->GetGoal().y - unit->currentPosition.y) / legnth;
		// Calculate the movement vector
		unit->forwardDirection = Coordsf(xdir, ydir);
		// Set the new tile to taken
		battleSceneReference->SetTileOccupancy(unit->GetDestination(), true);
		// release the old tile
		battleSceneReference->SetTileOccupancy(unit->currentTile, false);
 	} 

}


void Moving::Step(Unit * unit, float dt)
{

	// If your stuck wait and update your position to your current (attempt to renter the path)
	if (unit->wait)
	{
		unit->currentPosition.y = unit->currentTile.y * TILESIZE;
		unit->currentPosition.x = unit->currentTile.x * TILESIZE;
 		Enter(unit);
		return;
	}

	// bools to check
	bool xComplete = false;
	bool yComplete = false;

	// Check if you have reached the x destination
	if (unit->forwardDirection.x != 0.0f)
	{

		unit->currentPosition.x += unit->forwardDirection.x * unit->speed * dt;

		if (std::abs(unit->currentPosition.x - unit->GetGoal().x) < UNITSPEED * dt)
		{
			unit->currentPosition.x = unit->GetGoal().x;
			xComplete = true;
		}
	}
	else
	{
		xComplete = true;
	}

	// Check if you have reached the y destination
	if (unit->forwardDirection.y != 0.0f)
	{

		unit->currentPosition.y += unit->forwardDirection.y * unit->speed * dt;

		if (std::abs(unit->currentPosition.y - unit->GetGoal().y) < UNITSPEED * dt)
		{
			unit->currentPosition.y = unit->GetGoal().y;
			yComplete = true;
		}
	}
	else
	{
		yComplete = true;
	}

	// Transition to check for enemies each step
	if (yComplete && xComplete)
	{
		unit->currentPosition.y = unit->GetGoal().y;
		unit->currentPosition.x = unit->GetGoal().x;
		unit->currentTile = unit->GetDestination();
		unit->UpdateDestination();
		unit->ChangeState(Moving::stateInstance);
	}

}


void Moving::Exit(Unit * unit)
{

	// Nothing to do 

}
