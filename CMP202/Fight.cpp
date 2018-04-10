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
	if (unit_->health < 0)
	{
		unit_->ChangeState(new Death(unit_));
		return;
	}

	// If the attack
	if (attackTimer > unit_->attckCooldown)
	{
		
		unit_->currentTarget->health -= 20;
		attackTimer = 0.0f;

	}

	if (unit_->currentTarget->health < 0)
	{
		unit_->ResetPath();
		unit_->ChangeState(new SearchAndDestoy(unit_));
		return;
	}


}

void Fight::Exit()
{
}
