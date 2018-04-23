#include "GameManager.h"

// Application includes
#include "Scene.h"
#include "LoadingScene.h"
#include "BattleScene.h"
// #include "MenuScene.h"

// Static initialisation
GameManager* GameManager::instance_ = nullptr;

GameManager::GameManager()
{

	// Game Manager will have a scene ready before the rendering thread is created

	// Set the current scene to the initial loading scene cast as a Scene
	currentScene_ = (Scene*)new LoadingScene();

	// Attempt to load the graphics for it 
	currentScene_->Init();

	// Set the instance 
	instance_ = this;

	// Call to load the first scene 
	SceneTransition(new BattleScene());

}


GameManager::~GameManager()
{

	// Call cleanup to ensure everything is cleaned (delete nullptr is defined)
	CleanUp();

}


GameManager * const GameManager::Create()
{

	// Create a new instance of gamemanger
	instance_ = new GameManager();

	// Return the instance reference
	return instance_;

}


void GameManager::CleanUp()
{

	// CleanUp the current scene in operation
	currentScene_->CleanUp();
	currentScene_ = nullptr;

}


bool GameManager::Update(float deltatime)
{

	// Lock the update access
	std::unique_lock<std::mutex> lock(sceneAccessUpdate);
	
	// Update to handle any user input
	currentScene_->HandleInput(deltatime);

	// Update the game systems 
	currentScene_->Update(deltatime);

	// Return keep running
	return true;

}


void GameManager::Render(sf::RenderWindow* window)
{

	// activate the window's context
	window->setActive(true);

	// This must be threadsafe

	while (window->isOpen())
	{

		// Clear the current window
		window->clear(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));

		// Lock the render access
		std::unique_lock<std::mutex> lock(sceneAccessRender);

		// render the base Tiles 
		currentScene_->Render(*window);

		// render UI elemets
		currentScene_->RenderUI(*window);
	
		// redering the scene 
		window->display();
	
	}

}


void GameManager::SceneTransition(Scene * scene)
{

	// Initialise the loading Scene with the game manager instance
	instance_->sceneToLoad_ = (Scene*)new LoadingScene();

	// Attempt to load the graphics for it 
	instance_->sceneToLoad_->Init();

	// Keep track of the existing scene and the new scene to be loaded
	instance_->sceneToCleanUp_ = instance_->currentScene_;
	
	// Switch the current scene to the new loading scene
	instance_->currentScene_ = instance_->sceneToLoad_;

	// set the scene that requires loading
	instance_->sceneToLoad_ = scene;

	// Create a thread to clean up the old and load the newscene
	instance_->loadingThread_ = new std::thread(&GameManager::LoadNewScene, instance_);

}


void GameManager::SwitchToLoadedScene()
{

	// Lock the update access
	std::unique_lock<std::mutex> scenelock(sceneAccessUpdate);
	// Lock the render access
	std::unique_lock<std::mutex> renderlock(sceneAccessRender);

	// take the reference to the loading scene
	sceneToCleanUp_ = currentScene_;

	// switch in the loaded scene
	currentScene_ = sceneToLoad_;

	// delete the loading scene
	sceneToCleanUp_->CleanUp();
	sceneToCleanUp_ = nullptr;

	// reset the scene to load pointer
	sceneToLoad_ = nullptr;

	// loading thread will delete
	loadingThread_ = nullptr;

}


void GameManager::LoadNewScene()
{

	// Clean up the old scene
	sceneToCleanUp_->CleanUp();
	sceneToCleanUp_ = nullptr;

	// Load the new scene 
	sceneToLoad_->Init();

	// Call to switch the scenes
	SwitchToLoadedScene();

}

