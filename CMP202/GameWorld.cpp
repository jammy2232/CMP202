#include "GameWorld.h"


GameWorld::GameWorld(int mapDimension): mapDimension_(mapDimension)
{

	// Allocate the memory for each layer of the map
	PathMap_.assign(mapDimension_*mapDimension_, true);
	TileMap_.assign(mapDimension_*mapDimension_, SpriteObject());
	unitMap_.assign(mapDimension_*mapDimension_, nullptr);

	// Randomly generate a new map 
	GenerateRandomWolrd();

}


GameWorld::~GameWorld()
{
}


bool GameWorld::CheckForUnit(sf::Vector2i tile)
{

	// Lock the data Access
	std::unique_lock<std::mutex> lock(lockmap_);

	if (unitMap_[tile.y * mapDimension_ + tile.x] == nullptr)
		return false;

	// otherwise a unit exists
	return true;

}


int GameWorld::GetUnitTeam(sf::Vector2i tile)
{

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// if there is no unit 
	if (!unitMap_[tile.y * mapDimension_ + tile.x])
	{
		// Default no unit
		return -1;
	}

	// Return a copy of the data the units team
	return (unitMap_[tile.y * mapDimension_ + tile.x]->GetTeam());

}


void GameWorld::AttackUnitOnTile(sf::Vector2i tile, int amount)
{

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// if there is no unit 
	if (!unitMap_[tile.y * mapDimension_ + tile.x])
	{
		// Default no unit
		return;
	}

	// Return a copy of the data the units team
	unitMap_[tile.y * mapDimension_ + tile.x]->Damage(amount);

}


std::vector<Unit*> GameWorld::GetUnitList()
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


bool GameWorld::SetUnitOnTile(Unit* unit, sf::Vector2i tile)
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


void GameWorld::FreeUnitFromTile(sf::Vector2i tile)
{

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// Remove the unit
	unitMap_[tile.y * mapDimension_ + tile.x] = nullptr;

	assert(unitMap_[tile.y * mapDimension_ + tile.x] == nullptr);

}


sf::Vector2i GameWorld::GetRandomFreeTile()
{

	// Lock the data access 
	std::unique_lock<std::mutex> lock(lockmap_);

	// define the map limit
	int mapLimit = mapDimension_ - 1;

	// Find a valid random spawn location for the unit
	sf::Vector2i coordinates = sf::Vector2i(rand() % mapLimit, rand() % mapLimit);

	// if the first is not valid - Keep attempting new locaitons until a valid one is found
	while (!(PathMap_[coordinates.y * mapDimension_ + coordinates.x] && unitMap_[coordinates.y * mapDimension_ + coordinates.x] == nullptr))
	{
		coordinates = sf::Vector2i(rand() % mapLimit, rand() % mapLimit);
	}

	// return the random component
	return coordinates;

}



void GameWorld::GenerateMap(SpriteRenderer& renderer)
{

	// For each tile in the map Render yourself
	for (auto tile : TileMap_)
	{
		renderer.DrawSprite(tile);
	}

}


void GameWorld::GenerateRandomWolrd()
{

	// Create a square map with Random Grass textures
	for (int i = 0; i < (mapDimension_ * mapDimension_); i++)
	{

		// Get a random texture (2 possible 100 or 101)
		int randomGrassTextureId = 100 + rand() % 2;
		// Store this in the map location 
		TileMap_[i].id = randomGrassTextureId;

	}


	// Add random trees and foliage on Per% of the map

	// Calculate the number of foliage
	int treeNumbers = mapDimension_ * mapDimension_ * 0.1f; // 10%

	// Randomly assigne these to locations
	for (int i = 0; i < treeNumbers; i++)
	{

		// Get a Random Tile ID
		int randomTileId_x = rand() % mapDimension_;
		int randomTileId_y = rand() % mapDimension_;

		// Get a random Tree tile 85 to 92 on the sprite sheet
		int randomTreeTile = 85 + rand() % 7;

		// Store this in the map location 
		TileMap_[randomTileId_y * mapDimension_ + randomTileId_x].id = randomTreeTile;
		// Areas are not walkable
		PathMap_[randomTileId_y * mapDimension_ + randomTileId_x] = false;

	}

	// Add a river down the middle with a crossing point

	// Go through the map and add the river ect
	for (int y = 0; y < (mapDimension_); y++)
	{

		for (int x = 0; x < (mapDimension_); x++)
		{

			// Check if you create Sand
			if (x == mapDimension_ * 9 / 20 || x == mapDimension_ * 11 / 20)
			{
				if (y > mapDimension_ * 9 / 20 && y < mapDimension_ * 11 / 20)
				{
					// Place Bridge
					TileMap_[y * mapDimension_ + x].id = 57;
					// Areas are walkable
					PathMap_[y * mapDimension_ + x] = true;
				}
				else
				{
					// Place Beach
					TileMap_[y * mapDimension_ + x].id = 45;
					// Areas are walkable
					PathMap_[y * mapDimension_ + x] = true;
				}
			}

			// Check if you create Water
			if (x > mapDimension_ * 9 / 20 && x < mapDimension_ * 11 / 20)
			{
				if (y > mapDimension_ * 9 / 20 && y < mapDimension_ * 11 / 20)
				{
					// Place Bridge
					TileMap_[y * mapDimension_ + x].id = 57;
					// Areas are walkable
					PathMap_[y * mapDimension_ + x] = true;
				}
				else
				{
					// Place Water
					TileMap_[y * mapDimension_ + x].id = 71;
					// Areas are not walkable
					PathMap_[y * mapDimension_ + x] = false;
				}

			}

			// Assign the actual screen position
			TileMap_[y * mapDimension_ + x].screen_position = sf::Vector2f(x*TILESIZE, y*TILESIZE);

		}

	}

}
