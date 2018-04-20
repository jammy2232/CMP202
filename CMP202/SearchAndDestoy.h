#pragma once

// base class and units
#include "Unit.h"

// SFML information
#include "SFML\System\Vector2.hpp"

// States and transitions
#include "GameWorld.h"
#include "AiState.h"
#include "Charge.h"

// Forward Declarations
class Unit;

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

