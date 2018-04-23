#include "Charge.h"


Charge::~Charge()
{
}


void Charge::Enter(GameWorld& world)
{

	// Request a path to the enemylocaton
	PathFinder::RequestPath(&unit_, unit_.GetCurrentTile(), unit_.GetTargetLocation());

}


void Charge::Step(GameWorld& world, float dt)
{

	// Check if the unit is dead 
	if (unit_.GetHealth() < 0)
	{

		// The unit is dead
		unit_.ChangeState(world, new Death(unit_));

		// No more actions required
		return;

	}

	// Check if the random path has been recieved
	if (unit_.waitingPathRequestFlag)
	{

		// if not do Nothing
		return;

	}
	else if (unit_.isPathEmpty())
	{
		// If you have a path but it's empty You reach where the enemy was and there not there - reset this to search
		unit_.ChangeState(world, new SearchAndDestoy(unit_));
		return;

	}
	else
	{

		if (MoveBlocked())
		{

			// Check if it's the enemy in the adjacent tile
			if (world.GetUnitTeam(unit_.GetTileDestination()) != unit_.GetTeam())
			{

				// Set the enemy location at the next ste
				unit_.SetTargetLocation(unit_.GetTileDestination());

				// Transision to fighting
				unit_.ChangeState(world, new Fight(unit_));

				// no more action required
				return;

			}
			else if (world.CheckForUnit(unit_.GetTileDestination()))
			{
				// Transision to fighting
				unit_.ChangeState(world, new SearchAndDestoy(unit_));

				// no more action required
				return;
			}

		}

		// Move the unit a block
		if (MoveTheUnit(world, dt))
		{

			// Validate the enemy is still on the path
			if (CheckPathIsValid(world))
			{
				// Keep moving along it
				return;
			}
			else
			{
				// target lost and Go back to hunting
				unit_.ChangeState(world, new SearchAndDestoy(unit_));
				return;
			}

		}

	}

}


void Charge::Exit(GameWorld& world)
{



}


bool Charge::CheckPathIsValid(GameWorld& world)
{

	// Get a copy of the path
	std::list<sf::Vector2i> path = unit_.CopyPath();

	// Check that the unit is till on the path
	for (auto step : path)
	{

		// Check if it's friend or foe
		if (world.CheckForUnit(step) && (world.GetUnitTeam(step) != unit_.GetTeam()))
		{
			return true;
		}

	}

	return false;

}

