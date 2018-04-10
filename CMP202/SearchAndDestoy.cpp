#include "SearchAndDestoy.h"


SearchAndDestoy::~SearchAndDestoy()
{
}


void SearchAndDestoy::Enter()
{

	// No check required

}


void SearchAndDestoy::Step(float dt)
{

	// Check if the unit is dead 
	if (unit_->health < 0)
	{
		unit_->ChangeState(new Death(unit_));
		return;
	}

	switch (state_)
	{

		case ENEMYCHECK: // Check if there are any enemies in range

			unit_->currentTarget = CheckForEnemies(5);

			if (unit_->currentTarget)
			{
								
				unit_->ChangeState(new Charge(unit_));
				return;

			}
			else
			{
				state_ = PATHWAITING;
				return;
			}

			break;

		case PATHWAITING: // Move to a random location

			// Wait for the path
			if (unit_->WaitngPath())
			{

				if (randomLocation == sf::Vector2i(-1, -1))
				{

					// Pick a random spot
					randomLocation = MoveToRandomLocation(10);

					// Request a path
					PathFinder::RequestPath(unit_, unit_->currentTile, randomLocation);

				}

				return;

			}
			else
			{

				// Check there is a vaild path
				if (unit_->GetFinalDestination() != randomLocation)
				{

					// Remove the node to empty the path
					unit_->UpdateDestination();

					// Reset Random
					randomLocation = sf::Vector2i(-1, -1);

					// The random location isn't valid restart the process
					state_ = PATHWAITING;
					return;

				}

				// get the first step of the destination
				currentDestination = unit_->GetDestination();

				assert(std::abs(currentDestination.x - unit_->currentTile.x) < 2);
				assert(std::abs(currentDestination.y - unit_->currentTile.y) < 2);

				state_ = MOVING;
				return;

			}

		break;

		case MOVING:

			// Make any initial calculations and checks
			if (MoveTheUnit(dt))
			{

				// Check if the paths empty
				if (unit_->WaitngPath())
				{
					randomLocation = sf::Vector2i(-1, -1);
				}

				state_ = ENEMYCHECK;
				return;

			}
			else
			{
				
				// If a significant time has passed
				if (MoveBlocked())
				{

					// Reset Random
					randomLocation = sf::Vector2i(-1, -1);
					state_ = ENEMYCHECK;
					return;

				}

			}

		break;

	}

}


void SearchAndDestoy::Exit()
{
}


Unit * SearchAndDestoy::CheckForEnemies(int range)
{

	// Search the grid local to this unit to find any enemies 
	for (int y = unit_->currentTile.y - range; y < unit_->currentTile.y + range; y++)
	{

		for (int x = unit_->currentTile.x - range; x < unit_->currentTile.x  + range; x++)
		{

			// Validate the positions are valid
			if (x > 0 && y > 0 && x < (unit_->mapSize()-1) && y < (unit_->mapSize()-1))
			{

				Unit* testUnit = unit_->gameBoard[y*unit_->mapSize() + x];

				// Check of there is an enemy unit
				if (testUnit != nullptr)
				{
					if (testUnit->team != unit_->team)
					{
						return testUnit;
					}

				}

			}

		}

	}

	return nullptr;
}


sf::Vector2i SearchAndDestoy::MoveToRandomLocation(int distance)
{

	// Add this to the current position cliped between a min an max value

	int randX = -distance + rand() % (2 * distance + 1);
	int randY = -distance + rand() % (2 * distance + 1);

	// Map Dimentions
	randX = std::max(0, std::min(unit_->mapSize() - 1, (unit_->currentTile.x + randX)));
	randY = std::max(0, std::min(unit_->mapSize() -1, (unit_->currentTile.x + randX)));
	
	// Return the target
	return sf::Vector2i(randX, randY);

}
