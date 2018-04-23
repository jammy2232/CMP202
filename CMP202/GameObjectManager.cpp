
#include "GameObjectManager.h"


// Create a unit manager with the capability to process a max unit cap
GameObjectManager::GameObjectManager(int ObjectCap, GameWorld& gameWorld) : gameWorld_(gameWorld)
{

	// Allocate the memory and assign the free slots in the vectors and queue
	objects_.assign(ObjectCap, nullptr);

	// Create the queue with all slots flagged as free
	for (int index = 0; index < ObjectCap; index++)
	{
		freeSlots_.push(index);
	}

}


GameObjectManager::~GameObjectManager()
{

	// Delete all the objects
	for (auto object : objects_)
	{
		delete object;
	}

	// empty the list
	objects_.clear();

}


void GameObjectManager::Update(float delta_time, SpriteRenderer & renderer)
{

	// Create the task list
	CreateTaskList();

	// Get a gameobject unit to process
	GameObject* object = GetObjectToProcess();

	// While there is objects still to be processed
	while (object)
	{

		// update the unit and Add the unit to the render queue
		renderer.DrawSprite(object->Update(gameWorld_, delta_time));

		// Attempt to get a new unit
		object = GetObjectToProcess();

	}

}


// Function to update of the unit states (Not thread Safe)
void GameObjectManager::CreateTaskList()
{

	// lock access to the unit list to spawn the new unit
	std::unique_lock<std::mutex> lock(objectList_);

	// If the list has already been populated do nothing
	if (!objectsToProcess_.empty())
	{
		return;
	}

	// Reference to track the iterator index
	int indexTracker = 0;

	// Update each of the Units
	for (auto object : objects_)
	{

		// If there is not object assigned skip this
		if (object)
		{

			// if the unit is active
			if (object->isActive())
			{
				std::unique_lock<std::mutex> lock(processingQueue_);
				objectsToProcess_.push(object);
			}
			else if (object->isDeleted())
			{

				// add this free slot to the free slots
				freeSlots_.push(indexTracker);

				// Delete the object 
				delete object;

				// set the object to nullptr
				objects_[indexTracker] = nullptr;

			}

		}

		// Increment the index 
		indexTracker++;

	}

}


// attempt to find a free location for the unit spawn and spawn it
bool GameObjectManager::AddObject(GameObject* newObject)
{

	// lock access to the unit list to spawn the new unit
	std::unique_lock<std::mutex> lock(objectList_);

	// if there is no free slots return false
	if (freeSlots_.empty())
		return false;

	// add the new unit to the free slot
	int index = freeSlots_.front();

	// pop the slot as used 
	freeSlots_.pop();

	// Add the unit to the container 
	objects_[index] = newObject;
	
	// Complete
	return true;

}


// Get a unit to process from the processing list
GameObject * GameObjectManager::GetObjectToProcess()
{

	// Lock access to the list of units to process
	std::unique_lock<std::mutex> lock(processingQueue_);

	// If the path is empty return nothing to process
	if (objectsToProcess_.empty())
		return nullptr;

	// If there is a unit to process return the pointer to the unit
	GameObject* objectToProcess = objectsToProcess_.front();

	// pop the list
	objectsToProcess_.pop();

	// return the unit
	return objectToProcess;

}