#pragma once
#include "AiState.h"

class Moving :
	public AiState
{
public:
	Moving();
	~Moving();

	void Enter(Unit* unit);
	void Step(Unit* unit);
	void Exit(Unit* unit);

private:



};

