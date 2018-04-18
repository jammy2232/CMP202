#include "Charge.h"


Charge::~Charge()
{
}


void Charge::Enter()
{

	// Clear any current path
	unit_->ResetPath();

	// Request a path to the enemylocaton
	PathFinder::RequestPath(unit_, unit_->GetCurrentTile(), unit_->GetCurrentTargetTile());

}


void Charge::Step(float dt)
{

	// Check if the unit is dead 
	if (unit_->GetHealth() < 0)
	{

		// The unit is dead
		unit_->ChangeState(new Death(unit_));

		// No more actions required
		return;

	}

	// Check if the random path has been recieved
	if (unit_->waitngPath)
	{

		// if not do Nothing
		return;

	}
	else if (unit_->PathEmpty())
	{
		// If you have a path but it's empty You reach where the enemy was and there not there - reset this to search
		unit_->ChangeState(new SearchAndDestoy(unit_));
		return;

	}
	else
	{

		if (MoveBlocked())
		{

			// Check if it's the enemy in the adjacent tile
			if (unit_->world_.GetUnitInfo(unit_->GetTileDestination()) == unit_->GetCurrentTarget())
			{

				// Update the reference to your enemy
				unit_->SetCurrentTarget(unit_->world_.GetUnitInfo(unit_->GetTileDestination()));

				// Transision to fighting
				unit_->ChangeState(new Fight(unit_));

				// no more action required
				return;

			}

		}

		// Move the unit a block
		if (MoveTheUnit(dt))
		{

			// Validate the enemy is still on the path
			if (CheckPathIsValid())
			{
				// Keep moving along it
				return;
			}
			else
			{
				// target lost and Go back to hunting
				unit_->ChangeState(new SearchAndDestoy(unit_));
				return;
			}

		}

	}

}


void Charge::Exit()
{



}


bool Charge::CheckPathIsValid()
{

	// Get a copy of the path
	std::list<sf::Vector2i> path = unit_->CopyPath();
	bool pathGoodFlag = false;

	// Check that the unit is till on the path
	for (auto step : path)
	{
		if (unit_->world_.GetUnitInfo(step) == unit_->GetCurrentTarget())
		{

			// Update the reference to your enemy
			unit_->SetCurrentTarget(unit_->world_.GetUnitInfo(step));
			//flag the path is good
			pathGoodFlag = true;

		}
	}

	return pathGoodFlag;

}

