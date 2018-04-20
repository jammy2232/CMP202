#include "SearchAndDestoy.h"


SearchAndDestoy::~SearchAndDestoy()
{
}


void SearchAndDestoy::Enter(GameWorld& world)
{

	// Assuming that there is a delay for a path pick a randomly location and request a path.

	// Pick a random spot within 5 tiles of the units current location
	sf::Vector2i randomLocation = MoveToRandomLocation(world, 5);

	// Request a path
	PathFinder::RequestPath(&unit_, unit_.GetCurrentTile(), randomLocation);

}


void SearchAndDestoy::Step(GameWorld& world, float dt)
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
		// If you have a path but it's empty - reset this state
		unit_.ChangeState(world, new SearchAndDestoy(unit_));
		return;

	}
	else
	{

		// Move the unit a block
		if (MoveTheUnit(world, dt))
		{

			// Check for enemies
			sf::Vector2i enemyLocation = CheckForEnemies(world,5);

			// if there is any enemies respond accordingly
			if (enemyLocation.x != -1)
			{

				//Charge
				unit_.SetTargetLocation(enemyLocation);
				unit_.ChangeState(world, new Charge(unit_));
				return;

			}

		}

		if (MoveBlocked())
		{

			// Check for enemies
			sf::Vector2i enemyLocation = CheckForEnemies(world, 5);

			// if there is any enemies respond accordingly
			if (enemyLocation.x != -1)
			{

				//Charge
				unit_.SetTargetLocation(enemyLocation);
				unit_.ChangeState(world, new Charge(unit_));
				return;

			}
			else
			{

				// If you have a path but it's empty - reset this state
				unit_.ChangeState(world, new SearchAndDestoy(unit_));

			}

		}

	}

}


void SearchAndDestoy::Exit(GameWorld& world)
{

	// Do nothing

}


sf::Vector2i SearchAndDestoy::CheckForEnemies(GameWorld& world, int range)
{

	// Get the units current tile
	sf::Vector2i currentTile = unit_.GetCurrentTile();
	sf::Vector2i enemyLocation(-1, -1);
	int mapDimension = world.GetMapDimension();
	int distanceSQR = range*range + range*range;

	// Search the grid local to this unit to find any enemies 

	// through the y 
	for (int y = currentTile.y - range; y < (currentTile.y + range); y++)
	{

		// through the x
		for (int x = currentTile.x - range; x < (currentTile.x + range); x++)
		{

			// Validate the positions i.e. within the world
			if (x > -1 && y > -1 && x < (mapDimension - 1) && y < (mapDimension - 1))
			{

				// Get the pointer to the unit on the tile
				const Unit* UnitOnTile = world.GetUnitInfo(sf::Vector2i(x, y));

				// Check of there is a valid unit there
				if (UnitOnTile != nullptr)
				{

					// Check the team of that unit
					if (UnitOnTile->GetTeam() != unit_.GetTeam())
					{

						int enemDist = (x - currentTile.x)*(x - currentTile.x) + (y - currentTile.y)*(y - currentTile.y);

						// Check if it's closer
						if (enemDist < distanceSQR)
						{
							distanceSQR = enemDist;
							enemyLocation = sf::Vector2i(x,y);
						}

					}

				}

			}

		}

	}

	// return the closest enemy
	return enemyLocation;

}


sf::Vector2i SearchAndDestoy::MoveToRandomLocation(GameWorld& world, int distance)
{

	// Get a random number to move in the x and y directions
	int randX = -distance + rand() % (2 * distance + 1);
	int randY = -distance + rand() % (2 * distance + 1);

	// Validate this position on the map relative to the units current position
	sf::Vector2i current = unit_.GetCurrentTile();

	// Calculate the new positons relative to the units positions
	randX = current.x + randX;
	randY = current.y + randY;

	// Validate these values using a lambda
	auto validate = [](int& coord, int mapDimension)
	{
		if (coord < 0) { coord = 0; }
		else if (coord >= mapDimension) { coord = mapDimension - 1; }
	};
	
	validate(randX, world.GetMapDimension());
	validate(randY, world.GetMapDimension());
	
	// Return the target
	return sf::Vector2i(randX, randY);

}
