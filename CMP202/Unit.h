#pragma once

// Global Variables - These are constant for the development environment
#include "GameSettings.h"

// std funciton/data includes
#include <list>
#include <mutex>
#include <vector>
#include <atomic>
#include <memory>

// Game Systems
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "PathFinder.h"
#include "AiState.h"
#include "GameWorld.h"


class Unit : public GameObject
{
public:

	// Declaration for teams with int representing sprite id for the specific team
	enum TEAM { NONE = 0, BLUE = 105, RED = 117 };

	// Construction
	Unit(sf::Vector2i position = sf::Vector2i(0,0), TEAM team = NONE);
	~Unit();

	// Handling state update and transition
	SpriteObject& Update(float delta_time);


	// Handling update of the states
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

	// This is a thread safe flag to track path finding requests
	std::atomic<bool> waitingPathRequestFlag;

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


	// Unit Rendering informaiton 
	SpriteObject sprite_;

};

