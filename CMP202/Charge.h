#pragma once

#include "AiState.h"
#include "Unit.h"
#include<list>

class Charge : public AiState
{

public:

	Charge(Unit* unit) { unit_ = unit; }
	~Charge();

	void Enter();
	void Step(float dt);
	void Exit();

private:

	// intenal function 
	bool CheckPathIsValid();

	// Internal states 
	enum STATE { PATHFINDNIG, MOVING };
	STATE state_ = PATHFINDNIG;


};

