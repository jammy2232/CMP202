#pragma once

// Core std systems
#include <vector>
#include <assert.h>

// Game Systems
#include "RenderManager.h"
#include "GameSettings.h" // work around for not getting tilemap size from xml

class MapGenerator
{

public:

	// Generates a map based on ints for a specified tilesheet
	MapGenerator(unsigned int mapDimension);
	~MapGenerator();

	// This allows the pathfinder to be setup
	std::vector<bool> GetStaticMapData() { assert(!staticMapData_.empty()); return staticMapData_; }

	// This parses the map information to the render manager
	RenderManager* CreateRenderSystem(std::string filename);

private:

	// Used to calculate the path the units need to follow
	std::vector<bool> staticMapData_;

	// Data for Rendering Only (Tile set on the ground) (true if walkable)
	std::vector<int> staticTileIdMap_;

	// Store the map size 
	unsigned int mapsize = 0;

};

