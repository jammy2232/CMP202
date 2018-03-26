#pragma once
#include "AiState.h"

#include "Moving.h"
class Moving;

class LookAround : public AiState
{
public:

	LookAround();
	~LookAround();

	void Enter(Unit* unit);
	void Step(Unit* unit, float dt);
	void Exit(Unit* unit);

	static LookAround* stateInstance;

};

