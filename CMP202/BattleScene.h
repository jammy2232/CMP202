#pragma once

// Game Settings 
#include "GameSettings.h"

// base class 
#include "Scene.h"

// std library functions
#include <vector>
#include <thread>
#include "Barrier.h"
#include <mutex>

// Units
#include "Unit.h"
#include "Projectile.h"
#include "SearchAndDestoy.h"

// Systems
#include "SpriteRenderer.h"
#include "GameWorld.h"
#include "GameObjectManager.h"
#include "PathFinder.h"

class PathFinder;
class Projectile;

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

	void Update(float dt);

	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);


	// Spawning 
	static void SpawnAttack(Projectile* attack);
	// static void SpawnUnit(Projectile* attack);

private:

	// Map size information 
	const int maxUnits = MAXUNITS;
	const int maxProjectiles = MAXUNITS * 2;
	const int mapSize = MAPSIZE;

	// Loading and preperation functions
	void SetUpViewWindows();

	// Thread safety for rendering
	std::mutex windowEditor_;

	// Worker Threads
	std::vector<std::thread*> workers_;
	
	// Pathfiding system
	std::vector<PathFinder*> pathfinder;

	// Game World
	GameWorld* world_;

	// The  Sprite Renderer
	SpriteRenderer* spriteRenderer_;

	// Game Object Manager
	GameObjectManager* units_;
	GameObjectManager* projectiles_;

	// Render Barrier
	Barrier* ProcessBarrier;

	// Static reference for spawning objects
	static BattleScene* instance_;

	// delta time reference
	void ProcessUnits();
	std::atomic<float> delta_time;

	// This controls the worker threads
	std::atomic<bool> play = true;

};

