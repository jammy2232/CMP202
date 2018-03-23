#pragma once

#include <list>
#include "PathFinder.h"
#include "AiState.h"
#include <mutex>
#include "Coordsf.h"
#include "Coordsi.h"

class Unit
{
public:

	Unit();
	~Unit();

	void Update();
	void ChangeState(AiState* newState);

	// Getters and Setters
	void SetPath(std::list<Coordsi> path);

private:

	// Ai Control
	int spriteId_;
	Coordsf currentPosition;
	Coordsi currentTile;
	std::list<Coordsi> path_;
	AiState* currentState_;

	// AI Stats
	int health;
	int attack;
	Unit* Opponent;

	// ThreadSafety
	std::mutex path_lock;

};

