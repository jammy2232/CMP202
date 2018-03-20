#include "BattleScene.h"

#include <thread>

BattleScene::BattleScene()
{

	// This is not for loading 
	loading = false;
	unloaded = false;

}

BattleScene::~BattleScene()
{
}

bool BattleScene::Init()
{

	// Initialise and load the map
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	// Flag that the initailsation has been complete
	loaded = true;

	// return to show error free;
	return true;

}

void BattleScene::CleanUp()
{

	unloaded = true;

}

Scene * BattleScene::transition()
{
	return nullptr;
}

void BattleScene::HandleInput()
{
}

void BattleScene::Update(float delta_time)
{
}

void BattleScene::Render(sf::RenderWindow & window)
{


}

void BattleScene::RenderUI(sf::RenderWindow & window)
{
}
