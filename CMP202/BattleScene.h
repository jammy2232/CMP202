#pragma once

// Game Settings 
#include "GameSettings.h"

// base class 
#include "Scene.h"

// std library functions
#include <vector>
#include <thread>

// Systems
#include "PathFinder.h"
#include "SpriteRenderer.h"
#include "GameWorld.h"
#include "GameObjectManager.h"

// Units
#include "Unit.h"
#include "SearchAndDestoy.h"


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
	const int maxUnits = 300;

	// Loading and preperation functions
	void SetUpViewWindows();

	// Thread safety for rendering
	std::mutex windowEditor_;
	
	// Pathfiding system
	PathFinder* pathfinder = nullptr;


	// Unit update threads
	// std::thread* unitupdate0 = nullptr;
	// std::thread* unitupdate1 = nullptr;

	// Game World
	GameWorld* world_;

	// The  Sprite Renderer
	SpriteRenderer* spriteRenderer_;

	// Game Object Manager
	GameObjectManager* units_;
	GameObjectManager* projectiles_;



};

