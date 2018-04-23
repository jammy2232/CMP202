#pragma once

// Standard function
#include <memory>
#include <thread>
#include <mutex>

// SFML Libraries
#include "SFML\Graphics.hpp"

// Forward delclarations
class Scene;

class GameManager
{
public:

	// Public Destructor
	~GameManager();

	// Controlling instance of GameManager
	static GameManager* const Create();

	// Core application controls
	void CleanUp();
	bool Update(float deltatime);
	void Render(sf::RenderWindow* window);

	// function to handle scene transition
	static void SceneTransition(Scene* scene);

private:

	// Reference to the current scene
	Scene* currentScene_ = nullptr;

	// Temporary scene and thread referencing to control loading between sceens
	Scene* sceneToCleanUp_ = nullptr;
	Scene* sceneToLoad_ = nullptr;
	std::thread* loadingThread_ = nullptr;
	std::mutex sceneAccessUpdate;
	std::mutex sceneAccessRender;

	// Functions for controlling the loading
	void SwitchToLoadedScene();
	void LoadNewScene();

	// Singleton pattern with no instanc reference
	static GameManager* instance_;
	GameManager();

};

