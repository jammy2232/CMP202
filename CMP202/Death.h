#pragma once
#include "AiState.h"

#include "Unit.h"

class Death : public AiState
{

public:

	Death(Unit& unit) : AiState(unit) {}
	~Death();

	void Enter(GameWorld& world);
	void Step(GameWorld& world, float dt);
	void Exit(GameWorld& world);

private:

	float timer_ = 0.0f; 

};

