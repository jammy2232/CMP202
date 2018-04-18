#include "MapGenerator.h"


MapGenerator::MapGenerator(unsigned int mapDimension): mapsize(mapDimension)
{

	// Checks 
	assert(mapDimension > 0);

	// Create a square map with Random Grass textures
	for (int i = 0; i < (mapDimension * mapDimension); i++)
	{

		// Get a random texture (2 possible 100 or 101)
		int randomGrassTextureId = 100 + rand() % 2;
		// Store this in the map location 
		staticTileIdMap_.push_back(randomGrassTextureId);
		// Assume all tiles are walkable
		staticMapData_.push_back(true);

	}


	// Add random trees and foliage on 10% of the map
	for (int i = 0; i < (mapDimension * mapDimension * 0.1f); i++)
	{

		// Get a Random Tile ID
		int randomTileId_x = rand() % mapDimension;
		int randomTileId_y = rand() % mapDimension;

		// Get a random Tree tile 85 to 92 on the sprite sheet
		int randomTreeTile = 85 + rand() % 7;

		// Store this in the map location 
		staticTileIdMap_[randomTileId_y * mapDimension + randomTileId_x] = randomTreeTile;
		// Areas are not walkable
		staticMapData_[randomTileId_y * mapDimension + randomTileId_x] = false;

	}

	// Add a river down the middle with a crossing point
	for (int y = 0; y < (mapDimension); y++)
	{

		for (int x = 0; x < (mapDimension); x++)
		{

			// Check if you create Sand
			if (x == mapDimension * 9 / 20 || x == mapDimension * 11 / 20)
			{
				if (y > mapDimension * 9 / 20 && y < mapDimension * 11 / 20)
				{
					// Place Bridge
					staticTileIdMap_[y * mapDimension + x] = 57;
					// Areas are walkable
					staticMapData_[y * mapDimension + x] = true;
				}
				else
				{
					// Place Beach
					staticTileIdMap_ [y * mapDimension + x] = 45;
					// Areas are walkable
					staticMapData_[y * mapDimension + x] = true;
				}
			}

			// Check if you create Water
			if (x > mapDimension * 9 / 20 && x < mapDimension * 11 / 20)
			{
				if (y > mapDimension * 9 / 20 && y < mapDimension * 11 / 20)
				{
					// Place Bridge
					staticTileIdMap_[y * mapDimension + x] = 57;
					// Areas are walkable
					staticMapData_[y * mapDimension + x] = true;
				}
				else
				{
					// Place Water
					staticTileIdMap_[y * mapDimension + x] = 71;
					// Areas are not walkable
					staticMapData_[y * mapDimension + x] = false;
				}

			}

		}

	}

}


MapGenerator::~MapGenerator()
{

	// Cleanup by destruct
	staticMapData_.clear();
	staticTileIdMap_.clear();

}


RenderManager* MapGenerator::CreateRenderSystem(std::string filename)
{

	// Create a new manager system
	RenderManager* manager = new RenderManager(staticMapData_.size(), filename);

	// Transfer the mapdata to this system for rendering
	for (int y = 0; y < mapsize; y++)
	{

		for (int x = 0; x < mapsize; x++)
		{

			// Create the entity position 
			RenderObject object;

			// Set it's x and y poition and sprite id
			object.screen_position = sf::Vector2f(x * TILESIZE, y * TILESIZE);
			object.id = staticTileIdMap_[y * mapsize + x];

			// Store the object
			manager->AddEntity(object);

		}
	}

	// return the map
	return manager;

}
