#pragma once
#include "AiState.h"

#include "Unit.h"

class Death : public AiState
{

public:

	Death(Unit* unit) { unit_ = unit; }
	~Death();

	void Enter();
	void Step(float dt);
	void Exit();

private:

	float timer = 0.0f; 

};

