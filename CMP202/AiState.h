#pragma once

#include "SFML\System\Vector2.hpp"
#include "Unit.h"
#include "GameWorld.h"

class Unit;
class GameWorld;

class AiState
{
public:
	
	AiState(Unit& unit) : unit_(unit) {}
	~AiState();

	virtual void Enter(GameWorld& world) {}
	virtual void Step(GameWorld& world, float dt) {}
	virtual void Exit(GameWorld& world) {}

	// defining the equals operator

protected:

	// References to unit needed for managing the state
	Unit& unit_;

	// functions fundamental and data to all states
	bool MoveTheUnit(GameWorld& world, float dt);
	bool MoveBlocked() { return blocked; }
	void TryMoveAgain() { blocked = false; blockingtime = 0.0f; }

private:

	// Functions for processing the movements
	void calculateMovementVector();
	bool MovePossible(GameWorld& world);
	bool incrementMovement(float dt);
	bool wait(float dt);

	// Variables for moving units
	float blockingtime = 0;
	bool blocked = false;
	sf::Vector2f forwardDirection;

	// enum states to track moving peices
	enum MOVINGSTATES {PREPARE, CHECK, MOVING, BLOCKED };
	MOVINGSTATES move_ = CHECK;

};

