#pragma once


#include "AiState.h"
#include "Unit.h"
#include "Projectile.h"


class Fight : public AiState
{

public:

	Fight(Unit* unit) { unit_ = unit; }
	~Fight();

	void Enter();
	void Step(float dt);
	void Exit();

private:

	float attackTimer = 0.0f;

};

