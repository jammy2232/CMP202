#pragma once

// SFML
#include "SFML\Graphics.hpp"

// std library
#include <atomic>

class Scene
{
public:

	// Constructor/Destructor
	Scene();
	~Scene();

	// Initialisation and flow control
	virtual bool Init() = 0;
	virtual void CleanUp() = 0;
	virtual Scene* transition() = 0;

	// Main Scene loop
	virtual void HandleInput() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;
	virtual void RenderUI(sf::RenderWindow& window) = 0;

	// Multithread Management
	virtual bool Loaded();
	virtual bool UnLoaded();
	bool IsLoadingScene() { return loading; }

protected:

	// Core Scene Componenets
	sf::View main_view;
	sf::View miniMap_View;

	// Data / Managers of data

	// Multithread
	std::atomic<bool> loaded;
	std::atomic<bool> unloaded;

	// Loading Thread ID
	bool loading;

};

