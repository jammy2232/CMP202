#pragma once

#include "Scene.h"
#include <thread>

class LoadingScene : Scene
{
public:

	LoadingScene();
	~LoadingScene();

	// Initialisation
	bool Init();
	void CleanUp();
	void SceneToLoad(Scene* newScene, Scene* oldScene);
	Scene* transition();

	// Main Scene loop
	void HandleInput();
	void Update(float delta_time);
	void Render(sf::RenderWindow& window);
	void RenderUI(sf::RenderWindow& window);

private:

	// Scene to load
	Scene* newScene_;
	Scene* oldScene_;

	// Loading scene parameters
	sf::Texture* loadingTexture;
	sf::Sprite* loadingSprite;
	sf::Texture* loadingWheelTexture;
	sf::Sprite* loadingWheelSprite;

	// Reference to the loading thread
	std::thread* loadThread;
	std::thread* unloadThread;

};

