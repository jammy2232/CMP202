#pragma once

#include "Scene.h"


class BattleScene :	public Scene
{
public:

	BattleScene();
	~BattleScene();

	// Initialisation
	bool Init();
	void CleanUp();
	
	// Main Scene loop
	void HandleInput();
	void Update(float delta_time);
	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);

private:




};

