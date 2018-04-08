#pragma once

#include "Scene.h"

// Standard library functions
#include <mutex>
#include <math.h>

class LoadingScene : Scene
{
public:

	LoadingScene();
	~LoadingScene();

	// Initialisation
	bool Init();
	void CleanUp();

	// Main Scene loop
	void HandleInput(float delta_time);
	void Update(float delta_time);
	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);

private:

	// Loading scene parameters
	sf::Texture* loadingBackgroundTexture_ = nullptr;
	sf::Sprite* loadingBackgroundSprite_ = nullptr;
	sf::Texture* loadingIconTexture_ = nullptr;
	sf::Sprite* loadingIconSprite_ = nullptr;

	// Scene manipulation
	float timer = 0.0f;

	// mutex for sprite protection
	std::mutex LoadingIconMutex;

};

