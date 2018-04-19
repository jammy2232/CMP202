#include "GameObjectManager.h"


// Create a unit manager with the capability to process a max unit cap
GameObjectManager::GameObjectManager(int ObjectCap, GameWorld& gameWorld) : gameWorld_(gameWorld)
{

	// Allocate the memory and assign the free slots in the vectors and queue
	objects_.assign(ObjectCap, GameObject());

	// Create the queue with all slots flagged as free
	for (int index = 0; index < ObjectCap; index++)
	{
		freeSlots_.push(index);
	}

}


GameObjectManager::~GameObjectManager()
{
}


// Function to update of the unit states (Not thread Safe)
void GameObjectManager::PreProcessing()
{

	// Reference to track the iterator index
	int indexTracker = 0;

	// Update each of the Units
	for (GameObject& object : objects_)
	{

		// if the unit is active
		if (object.isActive())
		{
			std::unique_lock<std::mutex> lock(processingQueue_);
			objectsToProcess_.push(&object);
		}
		else if (object.isDeleted())
		{
			// lock access to the unit list to spawn the new unit
			std::unique_lock<std::mutex> lock(objectList_);

			// add this free slot to the free slots
			freeSlots_.push(indexTracker);

			// Update the delete flag
			object.setDeleted();

		}
		
		// Increment the index 
		indexTracker++;

	}

}


void GameObjectManager::Update(float delta_time, SpriteRenderer& renderer)
{

	// attmept to get a unit to process
	GameObject* object = GetObjectToProcess();

	while (object)
	{	

		// update the unit and Add the unit to the render queue
		renderer.DrawSprite(object->Update(delta_time));

		// Attempt to get a new unit
		object = GetObjectToProcess();

	}

}


// attempt to find a free location for the unit spawn and spawn it
bool GameObjectManager::SpawnObject(GameObject newUnit)
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
	objects_[index] = newUnit;

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

