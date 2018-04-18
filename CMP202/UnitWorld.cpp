#include "UnitWorld.h"


UnitWorld::UnitWorld(int mapDimension, int NumberOfUnits, std::vector<bool>& StaticMap, RenderManager& unitRenderer): mapDimension_(mapDimension)
{

	// Populate the unit map with placeholder positions
	unitMap_.assign(mapDimension*mapDimension, nullptr);

	// variable for team selecetion
	Unit::TEAM teamSelection = Unit::TEAM::BLUE;

	// Spawn Units in random locations
	for (int i = 0; i < NumberOfUnits; i++)
	{

		// Find a valid random spawn location for the unit
		sf::Vector2i coordinates = sf::Vector2i(rand() % (mapDimension - 1), rand() % (mapDimension - 1));

		// if the first is not valid - Keep attempting new locaitons until a valid one is found
		while (!(StaticMap[coordinates.y * mapDimension + coordinates.x] && unitMap_[coordinates.y * mapDimension + coordinates.x] == nullptr))
		{
			coordinates = sf::Vector2i(rand() % (mapDimension - 1), rand() % (mapDimension - 1));

		}

		// Assign a team (evenly)
		if (teamSelection == Unit::TEAM::BLUE)
		{
			teamSelection = Unit::TEAM::RED;
		}
		else
		{
			teamSelection = Unit::TEAM::BLUE;
		}

		// Create a new unit
		Unit* newUnit = new Unit(coordinates, teamSelection, *this);
		newUnit->ChangeState(new SearchAndDestoy(newUnit));

		// Place the unit on the map 
		unitMap_[newUnit->GetCurrentTile().y * mapDimension + newUnit->GetCurrentTile().x] = newUnit;

		// initial sync with the renderer 
		newUnit->SetEntityId(unitRenderer.AddEntity(newUnit->GetSpriteInfo()));

	}


}


UnitWorld::~UnitWorld()
{
}


bool UnitWorld::CheckForUnit(sf::Vector2i tile)
{

	// Lock the data Access
	std::unique_lock<std::mutex> lock(lockmap_);

	if (unitMap_[tile.y * mapDimension_ + tile.x] == nullptr)
		return false;

	// otherwise a unit exists
	return true;

}


Unit* UnitWorld::GetUnitInfo(sf::Vector2i tile)
{

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// Return a copy of the data
	return (unitMap_[tile.y * mapDimension_ + tile.x]);

}


std::vector<Unit*> UnitWorld::GetUnitList()
{

	// setup a temporary store
	std::vector<Unit*> unitList;

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// Go through the map and short list all the placed units 
	for (auto unit : unitMap_)
	{
		if (unit)
			unitList.push_back(unit);
	}

	// return a copy of references to all the units 
	return unitList;

}


bool UnitWorld::SetUnitOnTile(Unit* unit, sf::Vector2i tile)
{

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// fail if the tile is currently occupied
	if (unitMap_[tile.y * mapDimension_ + tile.x])
	{
		return false;
	}

	// otherwise add the unit
	unitMap_[tile.y * mapDimension_ + tile.x] = unit;
	return true;


}


void UnitWorld::FreeUnitFromTile(sf::Vector2i tile)
{

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// Remove the unit
	unitMap_[tile.y * mapDimension_ + tile.x] = nullptr;

}