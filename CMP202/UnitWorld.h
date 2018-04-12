#pragma once

#include <mutex>
#include <vector>
#include "Unit.h"
#include "RenderManager.h"
#include <assert.h>
#include "SearchAndDestoy.h"
#include <atomic>

// Purpose is to provide thread safe access to the map of unit positions

class UnitWorld
{
public:

	UnitWorld(int mpatDimensions, int NumberOfUnits, std::vector<bool>& StaticMap, RenderManager& unitRenderer);
	~UnitWorld();

	// Get Unit information
	bool CheckForUnit(sf::Vector2i tile);
	Unit* GetUnitInfo(sf::Vector2i tile);
	std::vector<Unit*> GetUnitList();

	// Get Map Information 
	int GetMapDimension() { return mapDimension_; }

private:

	// Protect access to the unit list 
	std::mutex lockmap_;
	std::vector<Unit*> unitMap_;

	// Map information
	int mapDimension_;

};

