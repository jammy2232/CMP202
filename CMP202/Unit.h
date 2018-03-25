#pragma once

#include <list>
#include "PathFinder.h"
#include "AiState.h"
#include <mutex>
#include "Coordsf.h"
#include "Coordsi.h"
#include "GameSettings.h"

class Unit
{
public:

	Unit();
	~Unit();

	void Update();
	void ChangeState(AiState* newState);

	// Getters and Setters (All Thread Safe)
	void SetPath(std::list<Coordsi> path);
	Coordsi GetDestination();


private:

	// Rendering informaiton 
	int spriteId_;
	Coordsf currentPosition;

	// AI Coontrol Information 
	Coordsi currentTile;
	std::list<Coordsi> path_;
	AiState* currentState_;

	// AI Stats
	int health;
	int attack;
	static float speed;
	Unit* Opponent;

	// ThreadSafety
	std::mutex path_lock;

};

