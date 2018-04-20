#pragma once

// Global Variables - These are constant for the development environment
#include "GameSettings.h"

// std includes
#include <list>
#include <vector>

// Thread protection for pathfiding information
#include <mutex>
#include <atomic>

// Game Systems
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "GameWorld.h"
#include "PathFinder.h"
#include "AiState.h"


class Unit : public GameObject
{
public:

	// Declaration for teams with int representing sprite id for the specific team
	enum TEAM { NONE = 0, BLUE = 105, RED = 117 };

	// Construction
	Unit(sf::Vector2i position = sf::Vector2i(0,0), TEAM team = NONE);
	~Unit();

	// Handling state update and transition returning the render request
	SpriteObject& Update(GameWorld& world, float delta_time);

	// Handling update of the states
	void ChangeState(GameWorld& world, AiState* newState);

	// Navigation controls for path access (all thread safe)
	void SetPath(std::list<sf::Vector2i> path);

	// This is a thread safe flag to track path finding requests
	std::atomic<bool> waitingPathRequestFlag = false;

	// Basic actions for accessing and modifying the path data 
	void UpdateDestination();
	bool isPathEmpty();
	std::list<sf::Vector2i> CopyPath();

	// Access to path information (all thread safe)
	sf::Vector2i GetTileDestination();
	sf::Vector2f GetPointDestination();

	// Get access to the units current information
	sf::Vector2i GetCurrentTile();
	sf::Vector2f GetCurrentPoint();
	void SetCurrentTile(sf::Vector2i tile);

	// Damaging a unit (Thread Safe access) accessed through the game map
	void Damage(int Amount) { health -= Amount; }
	int GetHealth() { return health; }
	TEAM GetTeam() const;

	// Update the sprite info
	void SetScreenPosition(sf::Vector2f position);
	void SetSpriteId(int id);

	// Enquire about the units last sighting
	void SetTargetLocation(sf::Vector2i location) {	enemyLocation_ = location;	}
	sf::Vector2i GetTargetLocation() { return enemyLocation_; }
	

private:

	// AI Control Information 
	std::mutex path_lock;
	std::list<sf::Vector2i> path_;

	// path information and Position Information
	sf::Vector2i tileDestination;
	sf::Vector2f pointDestination;

	// last spotted enemy location
	sf::Vector2i enemyLocation_;

	// Current state controlling this units data
	AiState* currentState_;

	// Unit Data and stats
	TEAM team_;
	sf::Vector2i currentTile;
	std::atomic<int> health = 100;

	// Unit Rendering informaiton 
	SpriteObject sprite_;

};

