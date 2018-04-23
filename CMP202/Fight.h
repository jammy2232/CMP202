#pragma once

// Game Systems
#include "AiState.h"
#include "Unit.h"
#include "Projectile.h"
#include "BattleScene.h"

// Other States
#include "Death.h"
#include "SearchAndDestoy.h"


class Fight : public AiState
{

public:

	Fight(Unit& unit) : AiState(unit) {}
	~Fight();

	void Enter(GameWorld& world);
	void Step(GameWorld& world, float dt);
	void Exit(GameWorld& world);

private:

	float attackTimer_ = 0.0f;

};

