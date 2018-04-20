#include "Fight.h"


Fight::~Fight()
{
}


void Fight::Enter(GameWorld& world)
{

	// Reset the timer 
	attackTimer_ = 0.0f;

}


void Fight::Step(GameWorld& world, float dt)
{

	// Update the attack timer
	attackTimer_ += dt;

	// Check if the unit is dead 
	if (unit_.GetHealth() < 0)
	{

		// The unit is dead
		unit_.ChangeState(world, new Death(unit_));

		// No more actions required
		return;

	}

	// Check the enemy is still valid
	if (!world.GetUnitInfo(unit_.GetTargetLocation()) || world.GetUnitInfo(unit_.GetTargetLocation())->GetTeam() == unit_.GetTeam())
	{

		// The enemy unit is gone
		unit_.ChangeState(world, new SearchAndDestoy(unit_));
		return;

	}

	// Attack the unit
	if (attackTimer_ > 1.0f)
	{
		
		// Implement this still
		// Projectile::SpawnProjectile(unit_->GetCurrentPoint(), unit_->GetCurrentTarget()->GetCurrentPoint(), unit_->GetCurrentTargetTile());
		attackTimer_ = 0.0f;

	}

}


void Fight::Exit(GameWorld& world)
{

	// Do Nothing

}
