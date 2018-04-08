#pragma once

// base class 
#include "Scene.h"

// std
#include <vector>
#include <thread>

// Battle Gameplay
#include "Unit.h"
#include "GameSettings.h"
#include "PathFinder.h"

// Generation and management systems
#include "RenderManager.h"
#include "MapGenerator.h"

// Initial unit state
#include "SearchAndDestoy.h"

// Forward delcarations
class PathFinder;
class Unit;
enum TEAM;

class BattleScene :	public Scene
{
public:

	BattleScene();
	~BattleScene();

	// Initialisation
	bool Init();
	void CleanUp();
	
	// Main Scene loop
	void HandleInput(float delta_time);
	void Update(float delta_time);
	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);

private:

	// Map size information 
	const int mapDimension = 50;
	const int maxUnits = 200;

	// Renderer Managers
	RenderManager* tileMapRenderer = nullptr;
	RenderManager* unitRenderer = nullptr;

	// Pathfiding system
	PathFinder* pathfinder;

	// Loading and preperation functions
	void SetUpViewWindows();

	// Create a vector to store all the unit locations
	std::vector<Unit*> units_;

	// Thread safety
	std::mutex windowEditor_;

};

