#include "Unit.h"

Unit::Unit(std::vector<Unit*>& board, int MapDimension) : gameBoard(board), mapDimension(MapDimension)
{

	health = UNITHEALTH;
	spriteInfo.id = 105;
	spriteInfo.x = 0.0f;
	spriteInfo.y = 0.0f;

}


Unit::~Unit()
{
}


void Unit::UpdateState(float dt)
{
	if (currentState_)
		currentState_->Step(dt);
}


void Unit::ChangeState(AiState * newState)
{

	// Exit the old
	if (currentState_)
	{
		currentState_->Exit();
		delete currentState_;
	}

	// Set the new
	currentState_ = newState;

	// Enter the new
	currentState_->Enter();

}

void Unit::SetInitialState(AiState * state)
{

	// Check no state exists
	assert(!currentState_);

	// assign the initial state
	currentState_ = state;

}


void Unit::SetPath(std::list<sf::Vector2i> path)
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

}


void Unit::ResetPath()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// Reset the path 
	path_.clear();

}


sf::Vector2i Unit::GetDestination()
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


sf::Vector2i Unit::GetFinalDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// check if a path exists
	if (path_.empty())
	{
		return currentTile;
	}

	// Get the next path desitnation 
	return path_.back();

}


sf::Vector2f Unit::GetGoal()
{

	sf::Vector2i temp = GetDestination();
	return sf::Vector2f((float)temp.x * TILESIZE, (float)temp.y * TILESIZE);

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

std::list<sf::Vector2i> Unit::CopyPath()
{
	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);
	return path_;

}
