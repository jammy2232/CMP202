#include "SearchAndDestoy.h"


SearchAndDestoy* SearchAndDestoy::stateInstance = nullptr;

SearchAndDestoy::SearchAndDestoy()
{
}


SearchAndDestoy::~SearchAndDestoy()
{
}


void SearchAndDestoy::Enter(Unit* unit)
{

	// Check for enemeies 

}


void SearchAndDestoy::Step(Unit* unit, float dt)
{

	// Move to a random locaiton 
	Coordsi randomLocation = MoveToRandomLocation(unit, 10);

	// Request a path
	PathFinder::RequestPath(unit, unit->currentTile, randomLocation);

	// transition to the move
	unit->ChangeState(Moving::stateInstance);


}


void SearchAndDestoy::Exit(Unit* unit)
{
}

Unit * SearchAndDestoy::CheckForEnemies(int range)
{
	return nullptr;
}

Coordsi SearchAndDestoy::MoveToRandomLocation(Unit* unit, int distance)
{
	// Add this to the current position cliped between a min an max value

	int randX = -distance + rand() % (2 * distance + 1);
	int randY = -distance + rand() % (2 * distance + 1);

	randX = std::max(0, std::min(MAPDIMENSIONS-1, (unit->currentTile.x + randX)));
	randY = std::max(0, std::min(MAPDIMENSIONS-1, (unit->currentTile.x + randX)));
	
	return Coordsi(randX, randY);
}
