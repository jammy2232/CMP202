#include "Charge.h"


Charge::~Charge()
{
}


void Charge::Enter()
{

	// Clear any current path
	unit_->ResetPath();

	// update the speed 
	speedMod_ = 3.0f;

	// Request a path to the enemylocaton
	PathFinder::RequestPath(unit_, unit_->currentTile, unit_->currentTarget->currentTile);

}


void Charge::Step(float dt)
{

	// Check if the unit is dead 
	if (unit_->health < 0)
	{
		unit_->ChangeState(new Death(unit_));
		return;
	}


	// Manage the state
	switch (state_)
	{

	case PATHFINDNIG:

		// Check the unit is still valid and on the current path
		if (!unit_->Active_)
		{
			unit_->ResetPath();
			unit_->currentTarget = nullptr;
			unit_->ChangeState(new SearchAndDestoy(unit_));
			return;
		}

		// Wait for the path
		if (!unit_->WaitngPath())
		{

			// get the first step of the destination
			currentDestination = unit_->GetDestination();

			// start moving 
			state_ = MOVING;

		}

		return;

	break;

	case MOVING:

		// Make any initial calculations and checks
		if (MoveTheUnit(dt))
		{

			// Check the unit is still valid and on the current path
			if (!unit_->Active_)
			{
				unit_->ResetPath();
				unit_->currentTarget = nullptr;
				unit_->ChangeState(new SearchAndDestoy(unit_));
				return;
			}

			// Check the unit is still on the path 
			if(!CheckPathIsValid())
			{
			
				PathFinder::RequestPath(unit_, unit_->currentTile, unit_->currentTarget->currentTile);
				state_ = PATHFINDNIG;
				return;

			}

			// get the first step of the destination
			currentDestination = unit_->GetDestination();

		}
		else
		{

			// Check if the target is in the next path
			if (unit_->gameBoard[unit_->GetDestination().y * unit_->mapSize() + unit_->GetDestination().x] == unit_->currentTarget)
			{
				// Move to attack
				unit_->ChangeState(new Fight(unit_));
				return;
			}

			// If a significant time has passed
			if (MoveBlocked())
			{

				// The unit can't get to that enemy atm
				unit_->ChangeState(new SearchAndDestoy(unit_));
				return;

			}

		}

		break;

	}

}


void Charge::Exit()
{

	// update the speed 
	speedMod_ = 1.0f;


}


bool Charge::CheckPathIsValid()
{

	// Get a copy of the path
	std::list<sf::Vector2i> path = unit_->CopyPath();
	bool pathGoodFlag = false;

	// Check that the unit is till on the path
	for (auto step : path)
	{
		if (unit_->gameBoard[step.y*unit_->mapSize() + step.x] == unit_->currentTarget)
		{
			pathGoodFlag = true;
		}
	}

	return pathGoodFlag;

}
