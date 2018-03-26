#pragma once
#include "AiState.h"

class Fighting : public AiState
{

public:
	Fighting();
	~Fighting();

	void Enter(Unit* unit);
	void Step(Unit* unit, float dt);
	void Exit(Unit* unit);

	static Fighting* stateInstance;

};

