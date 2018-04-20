#pragma once

// work around for not getting tilemap size from xml
#include "GameSettings.h" 

// Sprite renderer
#include "SpriteRenderer.h"

// Core std systems
#include <vector>

class MapGenerator
{

public:

	// Generates a map based on ints for a specified tilesheet
	MapGenerator(unsigned int mapDimension);
	~MapGenerator();

	// This allows the pathfinder to be setup
	std::vector<bool> GetStaticMapData() { assert(!staticMapData_.empty()); return staticMapData_; }

private:

	// Used to calculate the path the units need to follow
	std::vector<bool> staticMapData_;

	// Data for Rendering Only (Tile set on the ground) (true if walkable)
	std::vector<int> staticTileIdMap_;

	// Store the map size 
	unsigned int mapsize = 0;

};

