#pragma once

#include "SFML\System\Vector2.hpp"

class Unit;

class AiState
{
public:
	
	AiState(Unit* unit  = nullptr) : unit_(unit) {}
	~AiState();

	virtual void Enter() {}
	virtual void Step(float dt) {}
	virtual void Exit() {}

protected:

	// References to unit needed for managing the state
	Unit* unit_;

	// functions fundamental and data to all states
	bool MoveTheUnit(float dt);
	bool MoveBlocked() { return blocked; }

private:

	// Functions for processing the movements
	void calculateMovementVector();
	bool MovePossible();
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

