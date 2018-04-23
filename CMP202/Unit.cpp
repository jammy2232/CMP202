#include "Unit.h"


Unit::Unit(sf::Vector2i position, TEAM team) : 
	team_(team),
	currentState_(nullptr)
{

	// Set the current tile position
	currentTile = position;

	// Setup the rendering information (i.e. appearance and actual screen position)
	sprite_.screen_position = sf::Vector2f(position.x * TILESIZE, position.y * TILESIZE);
	sprite_.id = (int)team_;

}


Unit::~Unit()
{

	delete currentState_;

}

void Unit::Init(GameWorld & world, AiState* gameState)
{

	// Setup the initial state 
	ChangeState(world, gameState);

}


SpriteObject& Unit::Update(GameWorld& world, float dt)
{

	// If there is a current state - Update it
	if (currentState_)
		currentState_->Step(world, dt);

	// Return the object to render
	return sprite_;

}


void Unit::ChangeState(GameWorld& world, AiState* newState)
{

	// Exit the old
	if (currentState_)
	{
		currentState_->Exit(world);
		delete currentState_;
	}

	// Set the new
	currentState_ = newState;

	// Enter the new
	currentState_->Enter(world);

}


void Unit::SetPath(std::list<sf::Vector2i> path)
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// Remove the old path
	path_.clear();

	// set the new path
	path_ = path;
	
	// Apply the new information
	if (!path.empty())
	{

		tileDestination = path.front();
		pointDestination = sf::Vector2f(tileDestination.x * TILESIZE, tileDestination.y * TILESIZE);

	}

	// flag that the path has been calculated
	waitingPathRequestFlag = false;

}


sf::Vector2i Unit::GetTileDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> state(path_lock);

	// Get the next path desitnation 
	return tileDestination;

}


sf::Vector2f Unit::GetPointDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> state(path_lock);

	// Get the next path desitnation 
	return pointDestination;

}


sf::Vector2i Unit::GetCurrentTile()
{

	return currentTile;

}


sf::Vector2f Unit::GetCurrentPoint()
{

	return sprite_.screen_position;

}


void Unit::SetCurrentTile(sf::Vector2i tile)
{

	currentTile = tile;

}


int Unit::GetTeam() const
{

	// Get the next path desitnation 
	return (int)team_;

}


void Unit::UpdateDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// check if a path exists
	if (path_.empty())
	{
		// The path is empty
		return;
	}

	// Apply the new information
	tileDestination = path_.front();
	pointDestination = sf::Vector2f(tileDestination.x * TILESIZE, tileDestination.y * TILESIZE);

	// Get the next path desitnation 
	path_.pop_front();

}


bool Unit::isPathEmpty()
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
