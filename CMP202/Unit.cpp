#include "Unit.h"

float Unit::speed = UNITSPEED;

Unit::Unit()
{

	health = UNITHEALTH;
	attack = UNITATTACK;
	wait = false;
	spriteId_ = 103;

}


Unit::~Unit()
{
}


void Unit::Update(float dt)
{
	if (currentState_)
		currentState_->Step(this, dt);

}


void Unit::ChangeState(AiState * newState)
{

	// Exit the old
	if(currentState_)
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

void Unit::UpdateDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// Get the next path desitnation 
	path_.pop_front();

}
