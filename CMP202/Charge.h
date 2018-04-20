#pragma once

#include "AiState.h"
#include "Unit.h"
#include<list>
#include "SearchAndDestoy.h"
#include "Fight.h"
#include "Death.h"

class Charge : public AiState
{

public:

	Charge(Unit& unit) : AiState(unit) {}
	~Charge();

	void Enter(GameWorld& world);
	void Step(GameWorld& world, float dt);
	void Exit(GameWorld& world);

private:

	// intenal function 
	bool CheckPathIsValid(GameWorld& world);

};

