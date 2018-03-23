#pragma once

class AiState
{
public:

	AiState();
	~AiState();

	virtual void Enter() = 0;
	virtual void Step() = 0;
	virtual void Exit() = 0;

};

