#pragma once

#include "SFML\Graphics.hpp"

class Scene
{
public:

	// Constructor/Destructor
	Scene();
	~Scene();

	// Initialisation
	virtual bool Init() = 0;
	virtual void CleanUp() = 0;

	// Main Scene loop
	virtual void HandleInput() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;
	virtual void RenderUI(sf::RenderWindow& window) = 0;

protected:

	// Core Scene Componenets
	sf::View main_view;
	sf::View miniMap_View;

	// Data / Managers of data

};

