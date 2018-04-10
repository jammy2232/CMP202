#pragma once

// std funciton/data includes
#include <list>
#include <mutex>
#include <vector>

// Game Systems
#include "PathFinder.h"
#include "AiState.h"
#include "RenderManager.h"
#include "BattleScene.h"

#include <memory>

// Global Varialbles
#include "GameSettings.h"

class Unit
{
public:

	Unit(std::vector<Unit*>& board, int MapDimension);
	~Unit();

	// Declaration for team sprites
	enum TEAM { BLUE = 105, RED = 117 };

	// Handling state information
	void UpdateState(float dt);
	void ChangeState(AiState* newState);

	// Setting initial state
	void SetInitialState(AiState* state);

	// Access to the battleScene for seeing unit maps
	std::vector<Unit*>& gameBoard;

	// Navigation controls for path access (all thread safe)
	void SetPath(std::list<sf::Vector2i> path);
	void ResetPath();
	void UpdateDestination();
	sf::Vector2i GetDestination();
	sf::Vector2i GetFinalDestination();
	sf::Vector2f GetGoal();
	bool WaitngPath();
	int mapSize() { return mapDimension; }
	std::list<sf::Vector2i> CopyPath();

	// PositionUpdate
	bool posDirty_;
	bool Active_ = true;

	// Rendering informaiton 
	RenderObject spriteInfo;
	int entityId;

	// Position Information
	sf::Vector2i currentTile;

	// Unit consistant state
	int health = 100;
	float attckCooldown = 1.0f;
	Unit* currentTarget = nullptr;
	TEAM team;

private:

	// AI Coontrol Information 
	std::list<sf::Vector2i> path_;
	int mapDimension = 0;
	AiState* currentState_ = nullptr;

	// ThreadSafety
	std::mutex path_lock;

};

