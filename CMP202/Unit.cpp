#include "Unit.h"

float Unit::speed = UNITSPEED;

Unit::Unit()
{

	health = UNITHEALTH;
	attack = UNITATTACK;
	wait = false;
	spriteId_ = 105;

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

	// Remove the old one
	path_.clear();

	if (path.empty())
	{
		path.push_front(currentTile);
	}

	// Set the new path
	path_ = path;

	// set the final destination
	finalDestination = path_.back();

}

Coordsi Unit::GetDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// check if a path exists
	if (path_.empty())
	{
		return currentTile;
	}
	
	// Get the next path desitnation 
	return path_.front();

}

Coordsi Unit::GetFinalDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// check if a path exists
	if (path_.empty())
	{
		return currentTile;
	}

	// Get the next path desitnation 
	return finalDestination;

}


Coordsf Unit::GetGoal()
{

	Coordsi temp = GetDestination();
	return Coordsf((float)temp.x * TILESIZE, (float)temp.y * TILESIZE);

}


void Unit::UpdateDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// check if a path exists
	if (path_.empty())
	{
		return;
	}

	// Get the next path desitnation 
	path_.pop_front();

}


bool Unit::WaitngPath()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);
	return path_.empty();

}
