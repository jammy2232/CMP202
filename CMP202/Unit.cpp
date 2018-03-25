#include "Unit.h"

float Unit::speed = UNITSPEED;


Unit::Unit()
{

	int health = UNITHEALTH;
	int attack = UNITATTACK;

}


Unit::~Unit()
{
}


void Unit::Update()
{

	currentState_->Step(this);

}


void Unit::ChangeState(AiState * newState)
{

	// Exit the old
	currentState_->Exit(this);

	// Set the new
	currentState_ = newState;

	// Enter the new
	currentState_->Enter(this);

}

void Unit::SetPath(std::list<Coordsi> path)
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);
	path_ = path;

}

Coordsi Unit::GetDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);
	
	// Get the next path desitnation 
	return path_.front();

}
