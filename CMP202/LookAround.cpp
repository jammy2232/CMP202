#include "LookAround.h"


LookAround* LookAround::stateInstance = nullptr;


LookAround::LookAround()
{
}


LookAround::~LookAround()
{
}


void LookAround::Enter(Unit * unit)
{
}


void LookAround::Step(Unit * unit, float dt)
{

	// Check the squres around the vision of the unit and respond
	unit->ChangeState(Moving::stateInstance);


}


void LookAround::Exit(Unit * unit)
{
}
