#pragma once

#include "AiState.h"
#include "Coordsi.h"

#include "Moving.h"

class SearchAndDestoy : public AiState
{
public:

	SearchAndDestoy();
	~SearchAndDestoy();

	void Enter(Unit* unit);
	void Step(Unit* unit, float dt);
	void Exit(Unit* unit);

	static SearchAndDestoy* stateInstance;

private:

	Unit* CheckForEnemies(int range);
	Coordsi MoveToRandomLocation(Unit* unit, int distance);

	// Check for any closeby Enemies 

	

};

