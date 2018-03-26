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
}


void SearchAndDestoy::Step(Unit* unit, float dt)
{
}


void SearchAndDestoy::Exit(Unit* unit)
{
}

Unit * SearchAndDestoy::CheckForEnemies(int range)
{
	return nullptr;
}

Coordsi SearchAndDestoy::MoveToRandomLocation(int distance)
{
	return Coordsi();
}
