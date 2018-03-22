#pragma once

#include "Scene.h"

// std
#include <vector>
#include <thread>

// XML Parsing
#include "rapidxml.hpp"
#include <fstream>

// Battle Gameplay
#include "Units.h"

class BattleScene :	public Scene
{
public:

	BattleScene();
	~BattleScene();

	// Initialisation
	bool Init();
	void CleanUp();
	Scene* transition();
	
	// Main Scene loop
	void HandleInput(float delta_time);
	void Update(float delta_time);
	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);

private:

	// reference to graphics objects
	sf::Texture spriteSheet_;
	std::vector<sf::Sprite> sprites_;

	// Data for Rendering Only (Tile set on the ground)
	std::vector<int> renderMap_;

	// Data for Rendering and Calculation
	std::vector<Units> units_;

	// Data for Pathfinding
	// storing the locations that units are currently blocking
	std::vector<bool> currentMapStatus_;
	// Used to calculate the path the units need to follow
	std::vector<bool> tileMapStatus_;

	// Loading and preperation functions
	bool LoadSpriteSheet();
	void SetUpViewWindows();

};

