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

	
	// Check if you can move (true for blocked)
	if (battleSceneReference->CheckIfTileIsOccupied(unit->currentTile))
	{

		std::unique_lock<std::mutex> lock(unit->path_lock);

		// Calculate the movement vector
		unit->forwardDirection = Coordsf(unit->currentPosition.x - unit->GetDestination().x, unit->currentPosition.y - unit->GetDestination().y);
		// Set the new tile to taken
		battleSceneReference->SetTileOccupancy(unit->GetDestination(), true);
		// release the old tile
		battleSceneReference->SetTileOccupancy(unit->currentTile, false);
		// set the waiting 
		unit->wait = false;
	
	} 
	else 
	{
		// Wait
		unit->wait = true;
	}

}


void Moving::Step(Unit * unit, float dt)
{

	if (!unit->wait)
	{

		// Move unit to location at speed 
		unit->currentPosition.x += unit->forwardDirection.x;
		unit->currentPosition.y += unit->forwardDirection.y;

		// bools to check
		bool xComplete = false;
		bool yComplete = true;

		// Check if you have reached the x destination
		if (unit->forwardDirection.x > 0.0f)
		{
			if (unit->currentPosition.x > unit->GetDestination().x)
			{
				unit->currentPosition.x = unit->GetDestination().x;
				xComplete = true;
			}
		}
		else if (unit->forwardDirection.x < 0.0f)
		{
			if (unit->currentPosition.x < unit->GetDestination().x)
			{
				unit->currentPosition.x = unit->GetDestination().x;
				xComplete = true;
			}
		}


		// Check if you have reached the y destination
		if (unit->forwardDirection.y > 0.0f)
		{
			if (unit->currentPosition.y > unit->GetDestination().y)
			{
				unit->currentPosition.y = unit->GetDestination().y;
				yComplete = true;
			}
		}
		else if (unit->forwardDirection.y < 0.0f)
		{
			if (unit->currentPosition.y < unit->GetDestination().y)
			{
				unit->currentPosition.y = unit->GetDestination().y;
				yComplete = true;
			}
		}

		// Transition to check for enemies each step
		if (yComplete && yComplete)
		{
			unit->UpdateDestination();
			unit->ChangeState(LookAround::stateInstance);
		}

	}
	else
	{
		Enter(unit);
	}

}


void Moving::Exit(Unit * unit)
{

	// Nothing to do 

}
