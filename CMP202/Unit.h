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

	// Navigation (all thread safe)
	void SetPath(std::list<Coordsi> path);
	void UpdateDestination();
	Coordsi GetDestination();
	Coordsi GetFinalDestination();
	Coordsf GetGoal();
	bool WaitngPath();
	std::atomic<bool> wait;

	// Rendering informaiton 
	int spriteId_;

	// Position Information (Not Thread safe)
	Coordsf currentPosition;
	Coordsf forwardDirection;
	Coordsi currentTile;

	
	// AI Stats (Not Thread safe)
	int health;
	int attack;
	static float speed;
	Unit* Opponent;

private:

	// AI Coontrol Information 
	std::list<Coordsi> path_;
	Coordsi finalDestination;
	AiState* currentState_;

	// ThreadSafety
	std::mutex path_lock;

};

