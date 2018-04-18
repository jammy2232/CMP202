#pragma once

// std funciton/data includes
#include <list>
#include <mutex>
#include <vector>
#include <atomic>
#include <memory>

// Game Systems
#include "PathFinder.h"
#include "AiState.h"
#include "RenderManager.h"
#include "UnitWorld.h"

// Global Varialbles
#include "GameSettings.h"

class UnitWorld;

class Unit
{
public:

	// Declaration for teams with int representing sprite id
	enum TEAM { BLUE = 105, RED = 117 };

	// Construction
	Unit(sf::Vector2i position, TEAM team, UnitWorld& world);
	
	// Destructor
	~Unit();

	// Handling state update and transition
	void UpdateState(float dt);
	void ChangeState(AiState* newState);

	// Navigation controls for path access (all thread safe)
	void SetPath(std::list<sf::Vector2i> path);
	std::list<sf::Vector2i> CopyPath();
	void ResetPath();
	void UpdateDestination();
	bool PathEmpty();

	// Access to path information (all thread safe)
	sf::Vector2i GetTileDestination();
	sf::Vector2i GetTileFinalDestination();
	sf::Vector2f GetPointDestination();
	sf::Vector2i GetCurrentTile();
	sf::Vector2f GetCurrentPoint();
	void SetCurrentTile(sf::Vector2i tile);

	// this is data set at a specific point (all thread safe)
	std::atomic<bool> waitngPath;

	// Position and rendering (assumed update required)
	std::atomic<bool> posDirty_ = false;
	std::atomic<bool> Active_ = true;

	// Communication with rendering 
	void SetSpriteInfo(RenderObject newspriteinfo);
	void SetScreenPosition(sf::Vector2f position);
	void SetSpriteId(int id);
	RenderObject GetSpriteInfo();
	int GetEntityId();
	void SetEntityId(int entitIdNumber);

	// Impact Unit
	void Damage(int Amount);
	int GetHealth() { return health; }

	// Enemy Management
	sf::Vector2i GetCurrentTargetTile();
	void SetCurrentTarget(Unit* Target);
	Unit* GetCurrentTarget();

	// Access to the world to see (This is already thread safe)
	UnitWorld& world_;

private:

	// AI Control Information 
	std::mutex path_lock;
	std::list<sf::Vector2i> path_;

	// path information and Position Information
	std::mutex current_lock;
	sf::Vector2i currentTile;
	std::mutex pathState_lock;
	sf::Vector2i tileDestination;
	sf::Vector2i tileFinalDestination;
	sf::Vector2f pointDestination;

	// Current state controlling this units data
	std::mutex state;
	AiState* currentState_;

	// Unit Data and stats
	TEAM team_;
	std::atomic<int> health = 100;

	// enemy target
	std::mutex enemy;
	sf::Vector2i currentTargetTile;
	Unit* currentTarget = nullptr;

	// Unit Rendering informaiton 
	std::mutex render_lock;
	RenderObject spriteInfo;
	int entityId;

};

