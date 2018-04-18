#include "Fight.h"


Fight::~Fight()
{
}


void Fight::Enter()
{

	// Reset the timer 
	attackTimer = 0.0f;

}


void Fight::Step(float dt)
{

	// Update the attack timer
	attackTimer += dt;

	// Check if the unit is dead 
	if (unit_->GetHealth() < 0)
	{

		// The unit is dead
		unit_->ChangeState(new Death(unit_));

		// No more actions required
		return;

	}

	// Check the enemy is still valid
	if (!unit_->world_.GetUnitInfo(unit_->GetCurrentTargetTile()))
	{

		// The enemy unit is gone
		unit_->ChangeState(new SearchAndDestoy(unit_));
		return;

	}

	// Attack the unit
	if (attackTimer > 1.0f)
	{
		
		Projectile::SpawnProjectile(unit_->GetCurrentPoint(), unit_->GetCurrentTarget()->GetCurrentPoint(), unit_->GetCurrentTargetTile());
		attackTimer = 0.0f;

	}

}


void Fight::Exit()
{
}
