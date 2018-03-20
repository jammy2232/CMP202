#pragma once

#include "Scene.h"


#include <vector>

class BattleScene :	public Scene
{
public:

	BattleScene();
	~BattleScene();

	// Initialisation
	bool Init();
	void CleanUp();
	Scene* transition();
	
	// Main Scene loop
	void HandleInput();
	void Update(float delta_time);
	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);

private:

	std::vector<sf::Texture*> tests;
	sf::Sprite* loadingSprite;

};

