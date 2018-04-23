#pragma once

// Game Includes
#include "GameObject.h"
#include "GameWorld.h"

// System Includes
#include "SpriteRenderer.h"

// std includes
#include <vector>
#include <queue>

class GameWorld;

class GameObjectManager
{

public:

	// Constuctor
	GameObjectManager(int ObjectCap, GameWorld& gameWorld);
	~GameObjectManager();

	// Process all the units
	void Update(float delta_time, SpriteRenderer& renderer);

	// Spawn a new Unit
	bool AddObject(GameObject* newObject);

private:

	// Vector to store the unit informaiton 
	std::mutex objectList_;
	std::vector<GameObject*> objects_;
	std::queue<int> freeSlots_;

	// Thread processing queue
	// Call to update the units to the queue for processing
	void CreateTaskList();

	std::mutex processingQueue_;
	std::queue<GameObject*> objectsToProcess_;

	// Functions to manage queue access
	GameObject* GetObjectToProcess();

	// Static Reference to the game world
	GameWorld& gameWorld_;

};