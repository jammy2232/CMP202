#pragma once

// SFML information
#include "SFML\System\Vector2.hpp"

// Game systems
#include "GameWorld.h"
#include "AiState.h"
#include "Unit.h"

// States and transitions#include "Charge.h"
#include "Charge.h"

class SearchAndDestoy : public AiState
{

public:

	SearchAndDestoy(Unit& unit) : AiState(unit) {}
	~SearchAndDestoy();

	void Enter(GameWorld& world);
	void Step(GameWorld& world, float dt);
	void Exit(GameWorld& world);

private:

	// Internal state functions
	sf::Vector2i CheckForEnemies(GameWorld& world, int range);
	sf::Vector2i MoveToRandomLocation(GameWorld& world, int distance);

};

