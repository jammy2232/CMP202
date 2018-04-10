#pragma once

// base class and units
#include "Unit.h"
#include "AiState.h"
#include "SFML\System\Vector2.hpp"

#include "Charge.h"

//State links
class Unit;

class SearchAndDestoy : public AiState
{
public:

	SearchAndDestoy(Unit* unit) { unit_ = unit; }
	~SearchAndDestoy();

	void Enter();
	void Step(float dt);
	void Exit();

private:

	// Internal state functions
	Unit* CheckForEnemies(int range);
	sf::Vector2i MoveToRandomLocation(int distance);	

	// Internal data
	sf::Vector2i randomLocation = sf::Vector2i(-1, -1);

	// Internal states 
	enum STATE { ENEMYCHECK, PATHWAITING, MOVING };
	STATE state_ = ENEMYCHECK;

};

