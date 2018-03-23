#include "Unit.h"


Unit::Unit()
{
}


Unit::~Unit()
{
}


void Unit::Update()
{

	currentState_->Step();

}


void Unit::ChangeState(AiState * newState)
{

	// Exit the old
	currentState_->Exit();

	// Set the new
	currentState_ = newState;

	// Enter the new
	currentState_->Enter();

}

void Unit::SetPath(std::list<Coordsi> path)
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);
	path_ = path;

}
