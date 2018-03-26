#pragma once

#include <list>
#include "PathFinder.h"
#include "AiState.h"
#include <mutex>
#include "Coordsf.h"
#include "Coordsi.h"
#include "GameSettings.h"

class AiState;

enum TEAM { BLUE, RED };

class Unit
{
public:

	Unit();
	~Unit();

	void Update(float dt);
	void ChangeState(AiState* newState);

	// Getters and Setters (All Thread Safe)
	void SetPath(std::list<Coordsi> path);
	Coordsi GetDestination();
	void UpdateDestination();
	bool wait;

	// Rendering informaiton 
	int spriteId_;
	Coordsf currentPosition;
	Coordsf forwardDirection;
	Coordsi currentTile;
	
	// AI Stats
	int health;
	int attack;
	static float speed;
	Unit* Opponent;

	// ThreadSafety
	std::mutex path_lock;

private:


	// AI Coontrol Information 
	std::list<Coordsi> path_;
	AiState* currentState_;

};

