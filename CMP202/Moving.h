#pragma once
#include "AiState.h"

// Other states 
#include "LookAround.h"
class LookAround;

class Moving : public AiState
{

public:

	Moving();
	~Moving();

	void Enter(Unit* unit);
	void Step(Unit* unit, float dt);
	void Exit(Unit* unit);

	static Moving* stateInstance;

};

