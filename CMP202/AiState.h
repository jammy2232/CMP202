#pragma once

#include <vector>
#include "BattleScene.h"

class AiState
{
public:

	AiState();
	~AiState();

	virtual void Enter(Unit* unit) = 0;
	virtual void Step(Unit* unit) = 0;
	virtual void Exit(Unit* unit) = 0;

	static void SetSceneToControl(BattleScene* scene) { battleSceneReference = scene; }



protected:

	// References to data items needed for managing the states 
	static BattleScene* battleSceneReference;

};

