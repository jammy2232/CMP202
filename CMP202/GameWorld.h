#pragma once

// Game Core Settings 
#include "GameSettings.h"

// std includes
#include <mutex>
#include <vector>

// Game System includes
#include "SpriteRenderer.h"
#include "Unit.h"

// forward declaration 
class Unit;

// Purpose is to provide thread safe access to the map of unit positions, pathfinding information and create a render the tile map

class GameWorld
{
public:

	GameWorld(int mapDimensions);
	~GameWorld();

	// Get Unit information
	bool CheckForUnit(sf::Vector2i tile);
	std::vector<Unit*> GetUnitList();

	// Functions on the units
	int GetUnitTeam(sf::Vector2i tile);
	void AttackUnitOnTile(sf::Vector2i tile, int amount);

	// Updating the world
	bool SetUnitOnTile(Unit* unit, sf::Vector2i tile);
	void FreeUnitFromTile(sf::Vector2i tile);

	// Setting of the wolrd
	sf::Vector2i GetRandomFreeTile();

	// This allows the pathfinder to be setup
	std::vector<bool> GetStaticPathfinderMap() { return PathMap_; }

	// Get Map Information 
	int GetMapDimension() { return mapDimension_; }

	// RenderMap
	void GenerateMap(SpriteRenderer& renderer);

private:

	// Spawning and creating the world
	void GenerateRandomWolrd();

	// Protect access to the unit list 
	std::mutex lockmap_;
	std::vector<Unit*> unitMap_;

	// Protected Access to the game world information
	// Used to calculate the path the units need to follow (true if walkable)
	std::vector<bool> PathMap_;
	// Data for Rendering Only (Tile set on the ground) 
	std::vector<SpriteObject> TileMap_;

	// Map information
	unsigned int mapDimension_;

};

