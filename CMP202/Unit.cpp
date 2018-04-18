#include "Unit.h"


Unit::Unit(sf::Vector2i position, TEAM team, UnitWorld & world) : world_(world), team_(team)
{

	// Set the current tile position
	currentTile = position;

	// Setup the rendering information (i.e. appearance and actual screen position)
	spriteInfo.screen_position = sf::Vector2f(position.x * TILESIZE, position.y * TILESIZE);
	spriteInfo.id = (int)team_;

}


Unit::~Unit()
{

	delete currentState_;

}


void Unit::UpdateState(float dt)
{

	// Lock access to modifying the state
	// std::unique_lock<std::mutex> lock(state);

	if (currentState_)
		currentState_->Step(dt);
}


void Unit::ChangeState(AiState * newState)
{

	// Lock access to modifying the state
	// std::unique_lock<std::mutex> lock(state);

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

		// Lock the path to be updated
		std::unique_lock<std::mutex> state(pathState_lock);

		tileDestination = path.front();
		tileFinalDestination = path.back();
		pointDestination = sf::Vector2f(tileDestination.x * TILESIZE, tileDestination.y * TILESIZE);

	}

	// flag that the path has been calculated
	waitngPath = false;

}


void Unit::ResetPath()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// Reset the path 
	path_.clear();

	// Lock the path to be updated
	std::unique_lock<std::mutex> state(pathState_lock);

	// Reset the path data
	tileDestination = sf::Vector2i(0, 0);
	tileFinalDestination = sf::Vector2i(0, 0);
	pointDestination = sf::Vector2f(0.0f, 0.0f);

}


sf::Vector2i Unit::GetTileDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> state(pathState_lock);

	// check if a path exists
	if (path_.empty())
	{
		return currentTile;
	}
	
	// Get the next path desitnation 
	return tileDestination;

}


sf::Vector2i Unit::GetTileFinalDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> state(pathState_lock);

	// check if a path exists
	if (path_.empty())
	{
		return currentTile;
	}

	// Get the next path desitnation 
	return tileFinalDestination;

}


sf::Vector2f Unit::GetPointDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> state(pathState_lock);

	// check if a path exists
	if (path_.empty())
	{
		return spriteInfo.screen_position;
	}

	// Get the next path desitnation 
	return pointDestination;

}


sf::Vector2i Unit::GetCurrentTile()
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(current_lock);
	return currentTile;

}

sf::Vector2f Unit::GetCurrentPoint()
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(render_lock);
	return spriteInfo.screen_position;

}


void Unit::SetCurrentTile(sf::Vector2i tile)
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(current_lock);
	currentTile = tile;

}


void Unit::SetSpriteInfo(RenderObject newspriteinfo)
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(render_lock);
	spriteInfo = newspriteinfo;

}


void Unit::SetScreenPosition(sf::Vector2f position)
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(render_lock);
	spriteInfo.screen_position = position;
	posDirty_ = true;

}


void Unit::SetSpriteId(int id)
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(render_lock);
	spriteInfo.id = id;
	posDirty_ = true;

}


RenderObject Unit::GetSpriteInfo()
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(render_lock);
	return spriteInfo;

}


int Unit::GetEntityId()
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(render_lock);
	return entityId;

}


void Unit::SetEntityId(int entitIdNumber)
{

	// Lock the access to be updated
	std::unique_lock<std::mutex> Set(render_lock);
	entityId = entitIdNumber;

}


void Unit::Damage(int Amount)
{

	// Lock the access to be updated
	health += Amount;

}


sf::Vector2i Unit::GetCurrentTargetTile()
{
	
	// Lock
	std::unique_lock<std::mutex> state(enemy);
	return currentTargetTile;
	
}


void Unit::SetCurrentTarget(Unit * Target)
{

	// Lock
	std::unique_lock<std::mutex> state(enemy);

	if (Target)
	{
		currentTarget = Target;
		currentTargetTile = Target->GetCurrentTile();
	}
	else
	{
		currentTarget = nullptr;
		currentTargetTile = sf::Vector2i(0, 0);
	}

}


Unit * Unit::GetCurrentTarget()
{
	std::unique_lock<std::mutex> state(enemy);
	return currentTarget;
}


void Unit::UpdateDestination()
{

	// Lock the path to be updated
	std::unique_lock<std::mutex> Set(path_lock);

	// check if a path exists
	if (path_.empty())
	{

		// Lock the path to be updated
		std::unique_lock<std::mutex> state(pathState_lock);

		// Reset the path data
		tileDestination = sf::Vector2i(0, 0);
		tileFinalDestination = sf::Vector2i(0, 0);
		pointDestination = sf::Vector2f(0.0f, 0.0f);
		return;

	}

	// Get the next path desitnation 
	path_.pop_front();

	// Apply the new information
	if (!path_.empty())
	{

		// Lock the path to be updated
		std::unique_lock<std::mutex> state(pathState_lock);

		tileDestination = path_.front();
		tileFinalDestination = path_.back();
		pointDestination = sf::Vector2f(tileDestination.x * TILESIZE, tileDestination.y * TILESIZE);

	}

}


bool Unit::PathEmpty()
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
